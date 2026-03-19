#!/bin/bash

set -euo pipefail
PARENT_DIR="$(cd "$(dirname "$0")" && pwd)"


TMP_LOG="./run_all.log"
: > "$TMP_LOG"

log_note() {
  # Print to stdout and append to master log
  local msg="$1"
  echo "$msg"
  echo "$msg" >> "$TMP_LOG"
}

ORIG_CC="$PARENT_DIR/check_eff_fid.cc.orig"
cp -p "$PARENT_DIR/check_eff_fid.cc" "$ORIG_CC"

cleanup() {
  # restore working copy (does not touch mix3)
  if [[ -f "$ORIG_CC" ]]; then
    cp -p "$ORIG_CC" "$PARENT_DIR/check_eff_fid.cc" 2>/dev/null || true
  fi
}
trap cleanup EXIT

set_weight_token() {
  local from="$1"
  local to="$2"
  # Replace whole-word token, avoid partial replacements.
  perl -pi -e "s/(?<![A-Za-z0-9_])\\Q$from\\E(?![A-Za-z0-9_])/$to/g" "$PARENT_DIR/check_eff_fid.cc"
}

set_sf_variant() {
  # Replace <token>[0] and <token>[1] with provided expressions.
  # Usage: set_sf_variant token expr0 expr1
  local token="$1"
  local expr0="$2"
  local expr1="$3"
  perl -pi -e "s/(?<![A-Za-z0-9_])\\Q${token}\\E\\[0\\](?![A-Za-z0-9_])/$expr0/g" "$PARENT_DIR/check_eff_fid.cc"
  perl -pi -e "s/(?<![A-Za-z0-9_])\\Q${token}\\E\\[1\\](?![A-Za-z0-9_])/$expr1/g" "$PARENT_DIR/check_eff_fid.cc"
}

apply_pu_variant() {
  # Usage: apply_pu_variant nom|up|dn
  local v="$1"
  # normalize first
  set_weight_token "pu_wt_sys_up" "pu_wt"
  set_weight_token "pu_wt_sys_dn" "pu_wt"
  if [[ "$v" == "up" ]]; then
    set_weight_token "pu_wt" "pu_wt_sys_up"
  elif [[ "$v" == "dn" ]]; then
    set_weight_token "pu_wt" "pu_wt_sys_dn"
  fi
}

apply_sf_variant() {
  # Generic SF variant applier.
  # Usage:
  #   apply_sf_variant <base_token> <variant:nom|up|dn> [k]
  # It replaces base_token[0/1] with:
  #   <base_token>_up[0/1][k] or <base_token>_down[0/1][k]
  local base="$1"
  local v="$2"
  local k="${3:-}"
  if [[ "$v" == "nom" ]]; then
    return 0
  fi
  if [[ -z "${k:-}" ]]; then
    echo "Error: apply_sf_variant needs k for base=$base v=$v" >&2
    exit 1
  fi
  if [[ "$v" == "up" ]]; then
    set_sf_variant "$base" "${base}_up[0][$k]" "${base}_up[1][$k]"
  elif [[ "$v" == "dn" ]]; then
    set_sf_variant "$base" "${base}_down[0][$k]" "${base}_down[1][$k]"
  else
    echo "Error: unknown SF variant: $v" >&2
    exit 1
  fi
}

run_root_and_get_ratio_and_den() {
  # Returns "ratio|denA"
  # isBs2=1: parse "BDT: a, b" => ratio=b/a and denA=a
  # isBs2=0: parse "BDT: c"    => ratio=c/denA_in, denA=denA_in
  local tag="$1"
  local isBs2_val="$2"
  local isRun3_val="$3"
  local denA_in="${4:-}"
  local log="$PARENT_DIR/.${tag}.log"
  : > "$log"
  echo "=== $tag ===" >> "$TMP_LOG"
  root -l -b -q -e "gROOT->ProcessLine(\".L check_eff_fid.cc\"); gROOT->ProcessLine(\"isBs2=${isBs2_val};\"); gROOT->ProcessLine(\"isRun3=${isRun3_val};\"); gROOT->ProcessLine(\"run();\");" \
    >> "$log" 2>&1
  cat "$log" >> "$TMP_LOG"

  local line
  line="$(awk 'BEGIN{IGNORECASE=1} /BDT[[:space:]]*:[[:space:]]*/{last=$0} END{print last}' "$log")"
  if [[ -z "${line:-}" ]]; then
    echo "Error: no 'BDT:' line found in log: $log" >&2
    exit 1
  fi

  python3 - <<PY
import re, sys
line = """$line"""
nums = re.findall(r"[-+]?(?:\\d+(?:\\.\\d*)?|\\.\\d+)(?:[eE][-+]?\\d+)?", line)
isBs2 = int("$isBs2_val")
denA_in = "$denA_in".strip()
if isBs2 == 1:
    if len(nums) < 2:
        raise SystemExit(f"Error: isBs2=1 expects 2 numbers in BDT line, got: {line!r}")
    a = float(nums[0]); b = float(nums[1])
    if a == 0: raise SystemExit("Error: a==0")
    print(f"{b/a}|{a}")
else:
    if len(nums) < 1:
        raise SystemExit(f"Error: isBs2=0 expects 1 number in BDT line, got: {line!r}")
    c = float(nums[0])
    if not denA_in:
        raise SystemExit("Error: isBs2=0 needs denA from isBs2=1 run, but denA_in is empty")
    a = float(denA_in)
    if a == 0: raise SystemExit("Error: denA==0")
    print(f"{c/a}|{a}")
PY
}

run_config_ratio() {
  # Run a config and return ratio only.
  # Arguments: tag isRun3 puVariant sfBase sfVariant sfK(optional)
  local tag="$1"
  local isRun3="$2"
  local puVar="$3"
  local sfBase="$4"
  local sfVar="$5"
  local sfK="${6:-}"

  # Fresh copy per config
  cp -p "$ORIG_CC" "$PARENT_DIR/check_eff_fid.cc"
  apply_pu_variant "$puVar"
  apply_sf_variant "$sfBase" "$sfVar" "$sfK"

  # isBs2=1 first (gets denA)
  local out1 ratio1 denA
  out1="$(run_root_and_get_ratio_and_den "${tag}_isBs21" 1 "$isRun3")"
  ratio1="${out1%%|*}"
  denA="${out1#*|}"

  # isBs2=0 (uses denA)
  local out0 ratio0
  out0="$(run_root_and_get_ratio_and_den "${tag}_isBs20" 0 "$isRun3" "$denA")"
  ratio0="${out0%%|*}"

  # Return two ratios on ONE line for bash read:
  #   <ratio_isBs2=1> <ratio_isBs2=0>
  printf "%s %s\n" "$ratio1" "$ratio0"
}

relerr() {
  python3 - <<PY
A=float("$1")
X=float("$2")
print((X-A)/A)
PY
}

relerr_pct_pm() {
  # usage: relerr_pct_pm up dn  (inputs are fractional, e.g. 0.001)
  python3 - <<PY
up=float("$1")*100.0
dn=float("$2")*100.0
print(f"{up:+.3f}%/{dn:+.3f}%")
PY
}

set_result() {
  # bash 3.2 compatible "map": store values in dynamically named vars
  # usage: set_result key value
  local key="$1"
  local val="$2"
  local var="RESULT_${key}"
  printf -v "$var" '%s' "$val"
}

get_result() {
  # usage: get_result key
  local key="$1"
  local var="RESULT_${key}"
  printf '%s' "${!var-}"
}

for isBs2 in 1 0; do
  :
done

# ---- Baseline (norm) ratios ----
for isRun3 in 0 1; do
  read -r norm_bs2_1 norm_bs2_0 < <(run_config_ratio "norm_isRun3${isRun3}" "$isRun3" "nom" "mu_medium_eff_sf" "nom")
  set_result "norm_isBs21_isRun3${isRun3}" "$norm_bs2_1"
  set_result "norm_isBs20_isRun3${isRun3}" "$norm_bs2_0"
  log_note "[norm] isRun3=$isRun3  eps2/eps1=${norm_bs2_1}  eps3/eps1=${norm_bs2_0}"
done

# ---- Pileup (pu) systematics ----
for isRun3 in 0 1; do
  read -r up_bs2_1 up_bs2_0 < <(run_config_ratio "pu_up_isRun3${isRun3}" "$isRun3" "up" "mu_medium_eff_sf" "nom")
  read -r dn_bs2_1 dn_bs2_0 < <(run_config_ratio "pu_dn_isRun3${isRun3}" "$isRun3" "dn" "mu_medium_eff_sf" "nom")

  A1="$(get_result norm_isBs21_isRun3${isRun3})"
  A0="$(get_result norm_isBs20_isRun3${isRun3})"

  e21_up="$(relerr "$A1" "$up_bs2_1")"
  e21_dn="$(relerr "$A1" "$dn_bs2_1")"
  e31_up="$(relerr "$A0" "$up_bs2_0")"
  e31_dn="$(relerr "$A0" "$dn_bs2_0")"

  set_result "Pileup_isBs21_isRun3${isRun3}_up" "$e21_up"
  set_result "Pileup_isBs21_isRun3${isRun3}_dn" "$e21_dn"
  set_result "Pileup_isBs20_isRun3${isRun3}_up" "$e31_up"
  set_result "Pileup_isBs20_isRun3${isRun3}_dn" "$e31_dn"

  log_note "[Pileup] isRun3=$isRun3"
  log_note "  norm  eps2/eps1=$A1  eps3/eps1=$A0"
  log_note "  up    eps2/eps1=$up_bs2_1  eps3/eps1=$up_bs2_0"
  log_note "  down  eps2/eps1=$dn_bs2_1  eps3/eps1=$dn_bs2_0"
  log_note "  rel   eps2/eps1=$(relerr_pct_pm "$e21_up" "$e21_dn")  eps3/eps1=$(relerr_pct_pm "$e31_up" "$e31_dn")"
done

# ---- MUON medium efficiency systematics (k=0..3) ----
row_name_for_k() {
  case "$1" in
    0) echo "MUON_medium_EFF_RECO_STAT" ;;
    1) echo "MUON_medium_EFF_RECO_STAT_LOWPT" ;;
    2) echo "MUON_medium_EFF_RECO_SYS" ;;
    3) echo "MUON_medium_EFF_RECO_SYS_LOWPT" ;;
    *) echo "MUON_medium_EFF_RECO_$1" ;;
  esac
}

for k in 0 1 2 3; do
  row="$(row_name_for_k "$k")"
  for isRun3 in 0 1; do
    read -r up_bs2_1 up_bs2_0 < <(run_config_ratio "${row}_up_k${k}_isRun3${isRun3}" "$isRun3" "nom" "mu_medium_eff_sf" "up" "$k")
    read -r dn_bs2_1 dn_bs2_0 < <(run_config_ratio "${row}_dn_k${k}_isRun3${isRun3}" "$isRun3" "nom" "mu_medium_eff_sf" "dn" "$k")

    A1="$(get_result norm_isBs21_isRun3${isRun3})"
    A0="$(get_result norm_isBs20_isRun3${isRun3})"

    e21_up="$(relerr "$A1" "$up_bs2_1")"
    e21_dn="$(relerr "$A1" "$dn_bs2_1")"
    e31_up="$(relerr "$A0" "$up_bs2_0")"
    e31_dn="$(relerr "$A0" "$dn_bs2_0")"

    set_result "${row}_isBs21_isRun3${isRun3}_up" "$e21_up"
    set_result "${row}_isBs21_isRun3${isRun3}_dn" "$e21_dn"
    set_result "${row}_isBs20_isRun3${isRun3}_up" "$e31_up"
    set_result "${row}_isBs20_isRun3${isRun3}_dn" "$e31_dn"

    log_note "[${row}] k=$k isRun3=$isRun3"
    log_note "  norm  eps2/eps1=$A1  eps3/eps1=$A0"
    log_note "  up    eps2/eps1=$up_bs2_1  eps3/eps1=$up_bs2_0"
    log_note "  down  eps2/eps1=$dn_bs2_1  eps3/eps1=$dn_bs2_0"
    log_note "  rel   eps2/eps1=$(relerr_pct_pm "$e21_up" "$e21_dn")  eps3/eps1=$(relerr_pct_pm "$e31_up" "$e31_dn")"
  done
done

# ---- MUON lowpt efficiency systematics (k=0..3) ----
row_name_for_k_lowpt() {
  case "$1" in
    0) echo "MUON_lowpt_EFF_RECO_STAT" ;;
    1) echo "MUON_lowpt_EFF_RECO_STAT_LOWPT" ;;
    2) echo "MUON_lowpt_EFF_RECO_SYS" ;;
    3) echo "MUON_lowpt_EFF_RECO_SYS_LOWPT" ;;
    *) echo "MUON_lowpt_EFF_RECO_$1" ;;
  esac
}

for k in 0 1 2 3; do
  row="$(row_name_for_k_lowpt "$k")"
  for isRun3 in 0 1; do
    read -r up_bs2_1 up_bs2_0 < <(run_config_ratio "${row}_up_k${k}_isRun3${isRun3}" "$isRun3" "nom" "mu_lowpt_eff_sf" "up" "$k")
    read -r dn_bs2_1 dn_bs2_0 < <(run_config_ratio "${row}_dn_k${k}_isRun3${isRun3}" "$isRun3" "nom" "mu_lowpt_eff_sf" "dn" "$k")

    A1="$(get_result norm_isBs21_isRun3${isRun3})"
    A0="$(get_result norm_isBs20_isRun3${isRun3})"

    e21_up="$(relerr "$A1" "$up_bs2_1")"
    e21_dn="$(relerr "$A1" "$dn_bs2_1")"
    e31_up="$(relerr "$A0" "$up_bs2_0")"
    e31_dn="$(relerr "$A0" "$dn_bs2_0")"

    set_result "${row}_isBs21_isRun3${isRun3}_up" "$e21_up"
    set_result "${row}_isBs21_isRun3${isRun3}_dn" "$e21_dn"
    set_result "${row}_isBs20_isRun3${isRun3}_up" "$e31_up"
    set_result "${row}_isBs20_isRun3${isRun3}_dn" "$e31_dn"

    log_note "[${row}] k=$k isRun3=$isRun3"
    log_note "  norm  eps2/eps1=$A1  eps3/eps1=$A0"
    log_note "  up    eps2/eps1=$up_bs2_1  eps3/eps1=$up_bs2_0"
    log_note "  down  eps2/eps1=$dn_bs2_1  eps3/eps1=$dn_bs2_0"
    log_note "  rel   eps2/eps1=$(relerr_pct_pm "$e21_up" "$e21_dn")  eps3/eps1=$(relerr_pct_pm "$e31_up" "$e31_dn")"
  done
done

fmt_pm() {
  python3 - <<PY
up=float("$1")*100.0
dn=float("$2")*100.0
print(f"{up:+.2f}%/{dn:+.2f}%")
PY
}

cell() {
  # cell row isBs2 isRun3
  local row="$1" isBs2="$2" isRun3="$3"
  local up dn
  up="$(get_result "${row}_isBs2${isBs2}_isRun3${isRun3}_up")"
  dn="$(get_result "${row}_isBs2${isBs2}_isRun3${isRun3}_dn")"
  fmt_pm "$up" "$dn"
}

print_row() {
  local row="$1"
  local label="$2"
  local e21_r2 e21_r3 e31_r2 e31_r3
  e21_r2="$(cell "$row" 1 0)"
  e21_r3="$(cell "$row" 1 1)"
  e31_r2="$(cell "$row" 0 0)"
  e31_r3="$(cell "$row" 0 1)"
  printf "%-30s | %-14s | %-14s || %-14s | %-14s\n" "$label" "$e21_r2" "$e21_r3" "$e31_r2" "$e31_r3"
}

echo ""
echo "============================== Systematics table =============================="
printf "%-30s | %-14s | %-14s || %-14s | %-14s\n" "Systematics" "ε2/ε1 (13TeV)" "ε2/ε1 (13.6TeV)" "ε3/ε1 (13TeV)" "ε3/ε1 (13.6TeV)"
printf "%-30s-+-%-14s-+-%-14s-++-%-14s-+-%-14s\n" "$(printf '%.0s-' {1..30})" "$(printf '%.0s-' {1..14})" "$(printf '%.0s-' {1..14})" "$(printf '%.0s-' {1..14})" "$(printf '%.0s-' {1..14})"

print_row "Pileup" "Pileup"
print_row "MUON_medium_EFF_RECO_STAT" "MUON_medium_EFF_RECO_STAT"
print_row "MUON_medium_EFF_RECO_STAT_LOWPT" "MUON_medium_EFF_RECO_STAT_LOWPT"
print_row "MUON_medium_EFF_RECO_SYS" "MUON_medium_EFF_RECO_SYS"
print_row "MUON_medium_EFF_RECO_SYS_LOWPT" "MUON_medium_EFF_RECO_SYS_LOWPT"
print_row "MUON_lowpt_EFF_RECO_STAT" "MUON_lowpt_EFF_RECO_STAT"
print_row "MUON_lowpt_EFF_RECO_STAT_LOWPT" "MUON_lowpt_EFF_RECO_STAT_LOWPT"
print_row "MUON_lowpt_EFF_RECO_SYS" "MUON_lowpt_EFF_RECO_SYS"
print_row "MUON_lowpt_EFF_RECO_SYS_LOWPT" "MUON_lowpt_EFF_RECO_SYS_LOWPT"
echo "===============================================================================" 
echo ""
echo "Done. Workdir kept at: $PARENT_DIR"
echo "Logs kept at:         $PARENT_DIR"
