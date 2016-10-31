
using namespace std;
namespace little_endian_io {

    template <typename Word>
    std::ostream& write_word(std::ostream& outs, Word value, unsigned size = sizeof ( Word)) {
        for (; size; --size, value >>= 8)
            outs.put(static_cast<char> (value & 0xFF));
        return outs;
    }
}

#include "Wav.h"

using namespace little_endian_io;


    //Starting the header:
    
    Wav::Wav(string nome) {
        f = ofstream(nome+".wav", ios::binary);

        // Write the file headers
        f << "RIFF----WAVEfmt "; // (chunk size to be filled in later)
        write_word(f, 16, 4); // no extension data
        write_word(f, 1, 2); // PCM - integer samples
        write_word(f, 2, 2); // two channels (stereo file)
        write_word(f, 44100, 4); // samples per second (Hz)
        write_word(f, 176400, 4); // (Sample Rate * BitsPerSample * Channels) / 8
        write_word(f, 4, 2); // data block size (size of two integer samples, one for each channel, in bytes)
        write_word(f, 16, 2); // number of bits per sample (use a multiple of 8)

        // Write the data chunk header
        data_chunk_pos = f.tellp();
        f << "data----"; // (chunk size to be filled in later)

    }

    void Wav::addSample(int sample) {
        write_word(f, sample, 2);
    }

    void Wav::endWav() {
        // (We'll need the final file size to fix the chunk sizes above)
        size_t file_length = f.tellp();

        // Fix the data chunk header to contain the data size
        f.seekp(data_chunk_pos + 4);
        write_word(f, file_length - data_chunk_pos + 8);

        // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
        f.seekp(0 + 4);
        write_word(f, file_length - 8, 4);

    }

