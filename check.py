#Reference:
#https://gist.github.com/briandeheus/9df32136c756227df4bfbff580a1aadd
import binascii
import struct
class Punk(object):
    _END_CHUNK_TYPE = 'IEND'
    def __init__(self):
        self._mode = None
        self._file = None
        self._output = None
        self._bytes_to_hide = None
        self._bytes_read = 0
    def getfile(self, input_file):
        self._mode = 'encode'
        self._file = open(input_file, 'rb+')
        self._read_bytes(8)
        self._read_next_chunk()
    def _read_bytes_as_hex(self, position):
        return self._read_bytes(position).encode('hex')
    def _read_bytes_as_ascii(self, position):
        return self._read_bytes(position).encode('ascii')
    def _read_bytes_as_int(self, position):
        return int(self._read_bytes_as_hex(position), 16)
    def _read_bytes(self, byte_count):
        self._bytes_read += byte_count
        return self._file.read(byte_count)
    def _rewind_bytes(self, byte_count):
        self._bytes_read -= byte_count
        self._file.seek(self._bytes_read)
    def _read_next_chunk(self):
        chunk_size = self._read_bytes_as_int(4)
        print 'Chunk size:', chunk_size
        chunk_type = self._read_bytes_as_ascii(4)
        print 'Chunk type:', chunk_type
        if self._mode == 'encode' and chunk_type == self._END_CHUNK_TYPE:
            print 'Reached EOF'
            self._file.close()
            return
        content = self._read_bytes(chunk_size)
        crc = self._read_bytes_as_hex(4)
        print 'CRC:', crc
        self._read_next_chunk()
def main():  
    punk=Punk()
    punk.getfile('0.png')
if __name__ == '__main__':
    main()
