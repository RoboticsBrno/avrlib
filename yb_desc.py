import struct, sys
from uuid import UUID

class And:
    def __init__(self, *children):
        self.children = children

    def store(self):
        return chr(len(self.children)) + ''.join((child.store() for child in self.children))

class Or:
    def __init__(self, *children):
        self.children = children

    def store(self):
        return chr(0x80 | len(self.children)) + ''.join((child.store() for child in self.children))

class Config:
    def __init__(self, guid, first_pipe, pipe_count, flags = 0, data=''):
        self.flags = flags
        self.guid = guid
        self.first_pipe = first_pipe
        self.pipe_count = pipe_count
        self.data = data

    def store(self):
        return (chr(0) + chr(self.flags) + self.guid.bytes + chr(self.first_pipe) + chr(self.pipe_count)
            + chr(len(self.data)) + self.data)

def make_yb_desc(device_guid, child):
    return (chr(1) # version
        + device_guid.bytes + child.store())

def to_c(s):
    res = []
    while s:
        chunk = s[:16]
        s = s[16:]
        res.append(' '.join(('0x%02x,' % ord(ch) for ch in chunk)))
        res.append('\n')
    return ''.join(res)

def adler16(s, mod=251):
    c0 = 1
    c1 = 0
    for ch in s:
        c0 = (c0 + ord(ch)) % mod
        c1 = (c1 + c0) % mod
    return s + chr(c0) + chr(c1)

class HgInfo:
    def __init__(self, hg_log, rev_hash, timestamp, zoffset):
        self.hg_log = hg_log
        self.rev_hash = rev_hash
        self.timestamp = timestamp
        self.zoffset = zoffset

def get_hg_info():
    import subprocess
    rev_hash = subprocess.check_output(['hg', 'log', '-r', '.', '--color=no', '--template', '{node|short}'])
    rev_date = subprocess.check_output(['hg', 'log', '-r', '.', '--color=no', '--template', '{date|hgdate}'])
    hg_log = subprocess.check_output(['hg', 'log', '-r', '.', '--color=no'])
    hg_log = [line for line in hg_log.split('\n') if line]

    from base64 import b16decode
    rev_hash = b16decode(rev_hash, casefold=True)
    timestamp, zoffset = [int(x) for x in rev_date.split(' ')]

    return HgInfo(hg_log, rev_hash, timestamp, zoffset)
