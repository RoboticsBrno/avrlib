class HexError(RuntimeError):
    pass

class HexRecord:
    def __init__(self, kind, address, data):
        self.kind = kind
        self.address = address
        self.data = data

    def __repr__(self):
        return 'HexRecord(%r, %r, %r)' % (self.kind, self.address, self.data)

    @staticmethod
    def parse(line):
        if len(line) < 11 or line[0] != ':':
            raise HexError("Invalid record format")
        byte_count = int(line[1:3], 16)
        if len(line) != 11 + byte_count * 2:
            raise HexError("Invalid record format")

        data = [int(line[2*i+1:2*i+3], 16) for i in xrange(1, byte_count + 5)]

        address = (data[0]<<8) | data[1]
        record_type = data[2]

        checksum = data[-1]
        real_checksum = (0x100 - ((sum(data[:-1]) + byte_count) & 0xff)) & 0xff
        if checksum != real_checksum:
            raise HexError("Invalid checksum")

        return HexRecord(record_type, address, ''.join((chr(ch) for ch in data[3:-1])))

class HexFile:
    def __init__(self, records):
        self.records = list(records)

        regs = []
        def _addregion(addr, data):
            for i in xrange(len(regs)):
                raddr, rdata = regs[i]
                if raddr + len(rdata) > addr and addr + len(data) > raddr:
                    raise HexError("Overlapping regions")
                if raddr + len(rdata) == addr:
                    regs[i] = (regs[i][0], regs[i][1] + data)
                    break
            else:
                regs.append((addr, data))

        base_addr = 0
        for rec in self.records:
            if rec.kind == 0:
                _addregion(rec.address, rec.data)
            elif rec.kind == 1:
                break
            elif rec.kind == 2:
                base_addr = rec.address * 0x10
            elif rec.kind == 3:
                pass
            elif rec.kind == 4:
                base_addr = rec.address * 0x10000
            elif rec.kind == 5:
                pass
            else:
                raise HexError("Unknown record type")

        regs.sort()
        normregs = []
        for reg in regs:
            if len(normregs) == 0 or normregs[-1][0] + normregs[-1][1] != reg[0]:
                normregs.append(reg)
            else:
                normregs[-1][1] += reg[1]

        self.regions = normregs

    def make_small_blocks(self, block_size, pad_value):
        class _Cur:
            pass
        cur = _Cur()
        cur.addr = 0
        cur.data = [chr(pad_value)]*block_size
        res = []

        def _align(addr):
            return addr - (addr % block_size)

        def _add(addr, data):
            if cur.addr != _align(addr):
                res.append((cur.addr, ''.join(cur.data)))
                cur.data = [chr(pad_value)]*block_size
                cur.addr = _align(addr)

            addr -= cur.addr
            for i in xrange(addr, addr+len(data)):
                cur.data[i] = data[i-addr]

        for addr, data in self.regions:
            next_block = _align(addr+block_size)
            _add(addr, data[:next_block-addr])
            data = data[next_block-addr:]
            addr = next_block

            while data:
                _add(addr, data[:block_size])
                addr += block_size
                data = data[block_size:]

        res.append((cur.addr, ''.join(cur.data)))
        return res

    def make_big_blocks(self, block_size, pad_value):
        last_addr = None
        res = []
        for addr, data in self.make_small_blocks(block_size, pad_value):
            if last_addr == addr:
                res[-1] = (res[-1][0], res[-1][1] + data)
            else:
                res.append((addr, data))
            last_addr = addr + block_size
        return res

def load(file):
    recs = []
    for line in file:
        line = line.split('#', 1)[0].strip()
        if not line:
            continue
        recs.append(HexRecord.parse(line))
    return HexFile(recs)

if __name__ == '__main__':
    with open(r'c:\devel\checkouts\shupito\shupito23\fw_main_xmega\Release\fw_main_xmega.hex', 'r') as fin:
        hf = load(fin)
    blocks = hf.make_big_blocks(256, 0xff)
