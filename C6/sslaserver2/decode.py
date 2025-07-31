import binascii

with open("iopress.key", 'rb') as f:
    lines = f.readlines()
lines = b"".join(lines[1:-1])
key = binascii.a2b_base64(lines)

res = ""

for b in key:
    res += "0x%02x," % b

res="u8_t key[]={"+res[:-1]+"};"
print(res)

with open("iopress.crt", 'rb') as f:
    lines = f.readlines()
lines = b"".join(lines[1:-1])
cert = binascii.a2b_base64(lines)
res = ""
for b in cert:
    res += "0x%02x," % b

res="u8_t cert[]={"+res[:-1]+"};"
print()
print(res)