#!/usr/bin/env python3
import argparse
import os.path
import re
import subprocess
import sys
import warnings

def parse_slot_info(slot_info_file):
  data = open(slot_info_file, "rb").read()
  slot_info_pattern = re.compile(b'\xba\xdb\xee\xef(.{4})(.{4})\xba\xdb\xee\xef', flags=re.MULTILINE)
  result = slot_info_pattern.search(data)
  kernel_header_address = int.from_bytes(result.group(1), "little")
  userland_header_address = int.from_bytes(result.group(2), "little")
  return kernel_header_address, userland_header_address

def parse_userland_header(userland_header_file):
  data = open(userland_header_file, "rb").read()
  header_pattern = re.compile(b'\xfe\xed\xc0\xde.{8}.{4}.{4}(.{4})(.{4})(.{4})(.{4})\xfe\xed\xc0\xde', flags=re.MULTILINE)
  result = header_pattern.search(data)
  external_apps_flash_start = int.from_bytes(result.group(1), "little")
  external_apps_flash_end = int.from_bytes(result.group(2), "little")
  external_apps_ram_start = int.from_bytes(result.group(3), "little")
  external_apps_ram_end = int.from_bytes(result.group(4), "little")
  return external_apps_flash_start, external_apps_flash_end, external_apps_ram_start, external_apps_ram_end

def extract_device_info(linker_script_file_name):
  # Extract information
  ram_address = 0x20000000
  try:
    subprocess.check_output(["dfu-util", "-a", "0", "-s", str(hex(ram_address)) + ":16:force", "-U", "slot_info.bin"])
    _,userland_header_address = parse_slot_info("slot_info.bin")
    subprocess.check_output(["rm", "slot_info.bin"])
    subprocess.check_output(["dfu-util", "-a", "0", "-s", str(hex(userland_header_address)) + ":40:force", "-U", "userland_header.bin"])
    external_apps_flash_start, external_apps_flash_end, external_apps_ram_start, external_apps_ram_end = parse_userland_header("userland_header.bin")
    subprocess.check_output(["rm", "userland_header.bin"])
  except:
    warnings.warn("Warning: a device should be plugged to extract the correct flash/RAM layout")
    # No device plugged, default value
    external_apps_flash_start = 0x90120000
    external_apps_flash_end = 0x903f0000
    external_apps_ram_start = 0x2001f494
    external_apps_ram_end = 0x20037000

  # Store information and generate linker script
  linker_script = open(linker_script_file_name, "w")
  linker_script.write("/* This file is auto-generated by get_device_information.py */\n\n")
  linker_script.write("EXTERNAL_APPS_FLASH_START = "+str(hex(external_apps_flash_start))+";\n")
  linker_script.write("EXTERNAL_APPS_FLASH_END = "+str(hex(external_apps_flash_end))+";\n")
  linker_script.write("EXTERNAL_APPS_RAM_START = "+str(hex(external_apps_ram_start))+";\n")
  linker_script.write("EXTERNAL_APPS_RAM_END = "+str(hex(external_apps_ram_end))+";\n")
  linker_script.close()

parser = argparse.ArgumentParser(description="Get device information over USB")
parser.add_argument('ld', metavar='file.ld', help='output LD file')

args = parser.parse_args()
extract_device_info(args.ld)
