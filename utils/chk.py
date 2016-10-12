import re

def get_ports(filename):
    ports = []
    f = open(filename, 'r')
    for line in f:
        md = re.search(r'\.(\S+)\s+\(.*\)', line)
        if md:
            port = md.group(1)
            if port in ports:
                print 'duplicate: {}'.format(port)
                return None
            ports.append(port)
    return set(ports)

if '__main__' == __name__:
    ac = get_ports('../src/anxious_capture_top.v')
    ec = get_ports('../qsys/anxious_capture/anxious_capture_inst.v')
    print ac - ec - set(['c0', 'c1', 'c2', 'c3', 'inclk0'])
    print ec - ac
