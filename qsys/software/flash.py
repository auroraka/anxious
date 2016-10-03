#!/usr/bin/python
import xml.etree.ElementTree as ET
import sys
import os
import subprocess

def get_make_var(name):
    return subprocess.check_output('make print-{}'.format(name), shell=True).strip().split('= ')[-1]

sopcinfo_dir = get_make_var('SOPC_DIR')
try:
    num_cpus = int(get_make_var('NUM_CPUS'))
except Exception as e:
    num_cpus = 1

root = ET.parse(sopcinfo_dir).getroot()

for cpu_id in range(num_cpus):
    cpu_module = root.find('./module[@name="nios2_gen2_{}"]'.format(cpu_id))
    instruction_slaves = cpu_module.find('./parameter[@name="instSlaveMapParam"]/value'.format(cpu_id)).text
    assignments = {}
    for assignment_ele in cpu_module.findall('./assignment'):
        assignments[assignment_ele.find('./name').text] = assignment_ele.find('./value').text
    flash_slave_params = ET.fromstring(instruction_slaves).find('./slave[@name="{}"]'.format(assignments['embeddedsw.configuration.resetSlave']))
    os.environ['FLASH_BASE'] = flash_slave_params.get('start')
    os.environ['FLASH_END'] = flash_slave_params.get('end')
    os.environ['FLASH_RESET'] = assignments['embeddedsw.CMacro.RESET_ADDR']
    os.system('make do_flash CPU_ID={}'.format(cpu_id))
