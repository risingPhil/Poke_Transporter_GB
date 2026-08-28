#!/bin/bash
# This script generates the payloads using gba-payload-generator
# and pack the content.
# The english payload will be stored just compressed.
# but the other payloads will be turned into BPS patches based on the english payload, which will then be packed into
# a compressed filecontainer
# This is all done to keep the file sizes as small as possible
# The script requires 2 arguments: 
# 1.) the path to the build dir.
# 2.) the path to the uncompressed RSEFRLG text table bin file
BUILD_DIR=$1
RSEFRLG_BIN_PATH=$2
RAW_PAYLOAD_DIR=$1/gba-payloads
BPS_PATCH_DIR=$1/bps-patches

ABSOLUTE_BASE_VARIANT=ruby_english_1_0.bin

games=("ruby" "sapphire" "emerald" "firered" "leafgreen")
langs=("english" "french" "german" "italian" "japanese" "spanish")

# This function uses the flips application to generate a BPS patch
create_patch(){
    flips --create --bps $1 $2 $3
}

mkdir -p $RAW_PAYLOAD_DIR
mkdir -p $BPS_PATCH_DIR
tools/gba-payload-generator/gba-payload-generator $RSEFRLG_BIN_PATH $RAW_PAYLOAD_DIR

# first copy our absolute base payload to to_compress
cp $RAW_PAYLOAD_DIR/script_$ABSOLUTE_BASE_VARIANT to_compress/
cp $RAW_PAYLOAD_DIR/section30_$ABSOLUTE_BASE_VARIANT to_compress/

# establish patches to transform the english ruby payloads first
# into the other games' english payloads
# This is done because I saw that various patches ended up 500 bytes or more
#
# Then, generate patches for the other languages/versions based on this specific base.
# So to obtain the final non-english version in PTGB, you'll have to apply 2 patches.
for i in "${games[@]}"
do
	gamebase_script_filename="script_${i}_english_1_0.bin"
	gamebase_script_patch_file=${gamebase_script_filename%.*}.bps
	gamebase_section30_filename="section30_${i}_english_1_0.bin"
	gamebase_section30_patch_file=${gamebase_section30_filename%.*}.bps
	gamebase_script_path="$RAW_PAYLOAD_DIR/$gamebase_script_filename"
	gamebase_section30_path="$RAW_PAYLOAD_DIR/$gamebase_section30_filename"
	
	create_patch "$RAW_PAYLOAD_DIR/script_$ABSOLUTE_BASE_VARIANT" "$gamebase_script_path" "$BPS_PATCH_DIR/$gamebase_script_patch_file"
	create_patch "$RAW_PAYLOAD_DIR/section30_$ABSOLUTE_BASE_VARIANT" "$gamebase_section30_path" "$BPS_PATCH_DIR/$gamebase_section30_patch_file"
	
	# Now generate the non-english patches
	# based on the gamebase_script_filename and gamebase_section30_filename
	for j in "${langs[@]}"
	do
		specific_script_identifier="script_${i}_${j}*"
		specific_section30_identifier="section30_${i}_${j}*"

		find $RAW_PAYLOAD_DIR -name $specific_script_identifier | while read filename; do
			file_basename=$(basename $filename)
			patch_file=${file_basename%.*}.bps

			#avoid diffing with itself
			if [[ -f "$BPS_PATCH_DIR/$patch_file" ]]; then
				continue
			fi
			
			echo "Creating patch for $filename based on $gamebase_script_path"
			create_patch "$gamebase_script_path" "$filename" "$BPS_PATCH_DIR/$patch_file"
		done
		
		find $RAW_PAYLOAD_DIR -name $specific_section30_identifier | while read filename; do
			file_basename=$(basename $filename)
			patch_file=${file_basename%.*}.bps

			#avoid diffing with itself
			if [[ -f "$BPS_PATCH_DIR/$patch_file" ]]; then
				continue
			fi
			
			echo "Creating patch for $filename based on $gamebase_section30_path"
			create_patch "$gamebase_section30_path" "$filename" "$BPS_PATCH_DIR/$patch_file"
		done
	done
done

# we want to fill up the containerdef files separately from the BPS file generation, because we want the containerdef file ordered by language.
# For optimal compression, we want to keep the same language game variants stored contiguously, because
# the translation texts from the RSEFRLG text table will be mostly the same. Therefore compression could leverage that redundancy.
for j in "${langs[@]}"
do
	for i in "${games[@]}"
	do
		script_identifier="script_${i}_${j}*"
		section30_identifier="section30_${i}_${j}*"
		find $BPS_PATCH_DIR -name $script_identifier | LC_ALL=C sort | while read filename; do
			echo $filename >> $BPS_PATCH_DIR/script_patches.containerdef
		done
		find $BPS_PATCH_DIR -name $section30_identifier | LC_ALL=C sort | while read filename; do
			echo $filename >> $BPS_PATCH_DIR/section30_patches.containerdef
		done
	done
done
