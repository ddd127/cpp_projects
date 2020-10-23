#define OLYMP_TEMPLATE
#ifdef OLYMP_TEMPLATE

#include <cassert>
#include <cstdio>
#include <algorithm>/** Fast allocation */

//to use fast allocation uncomment next line
#define FAST_ALLOCATOR_MEMORY 100000000
#ifdef FAST_ALLOCATOR_MEMORY
int allocator_pos = 0;
char allocator_memory[(int) FAST_ALLOCATOR_MEMORY];
inline void *operator new(size_t n)
{
    char *res = allocator_memory + allocator_pos;
    allocator_pos += n;
    assert(allocator_pos <= (int)FAST_ALLOCATOR_MEMORY);
    return (void *)res;
}
inline void operator delete(void *) noexcept {}
//inline void * operator new [] ( size_t ) { assert(0); }
//inline void operator delete [] ( void * ) { assert(0); }
#endif

//to use file io uncomment next line
#define FILE_IO

/** Fast input-output */

#ifdef FILE_IO

FILE *inpt = fopen("input.txt", "r");
#define in_source inpt
FILE *outpt = fopen("output.txt", "w");
#define out_source outpt

#else

#define in_source stdin
#define out_source stdout

#endif

template<class T = int>
inline T readInt();

inline double readDouble();

inline int readUInt();

inline int readChar(); // first non-blank character
inline void readWord(char *s);

inline bool readLine(char *s); // do not save '\n'
inline bool isEof();

inline int getChar();

inline int peekChar();

inline bool seekEof();

inline void skipBlanks();

template<class T>
inline void writeInt(T x, char end = 0, int len = -1);

inline void writeChar(int x);

inline void writeWord(const char *s);

inline void writeDouble(double x, int len = 0); // works correct only for |x| < 2^{63}
inline void flush();

static struct buffer_flusher_t {
    ~buffer_flusher_t() {
        flush();
    }
} buffer_flusher;

/** Read */

static const int buf_size = 4096;

static unsigned char buf[buf_size];
static int buf_len = 0, buf_pos = 0;

inline bool isEof() {
    if (buf_pos == buf_len) {
        buf_pos = 0, buf_len = fread(buf, 1, buf_size, in_source);
        if (buf_pos == buf_len)
            return 1;
    }
    return 0;
}

inline int getChar() {
    return isEof() ? -1 : buf[buf_pos++];
}

inline int peekChar() {
    return isEof() ? -1 : buf[buf_pos];
}

inline bool seekEof() {
    int c;
    while ((c = peekChar()) != -1 && c <= 32)
        buf_pos++;
    return c == -1;
}

inline void skipBlanks() {
    while (!isEof() && buf[buf_pos] <= 32U)
        buf_pos++;
}

inline int readChar() {
    int c = getChar();
    while (c != -1 && c <= 32)
        c = getChar();
    return c;
}

inline int readUInt() {
    int c = readChar(), x = 0;
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    return x;
}

template<class T>
inline T readInt() {
    int s = 1, c = readChar();
    T x = 0;
    if (c == '-')
        s = -1, c = getChar();
    else if (c == '+')
        c = getChar();
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    return s == 1 ? x : -x;
}

inline double readDouble() {
    int s = 1, c = readChar();
    double x = 0;
    if (c == '-')
        s = -1, c = getChar();
    while ('0' <= c && c <= '9')
        x = x * 10 + c - '0', c = getChar();
    if (c == '.') {
        c = getChar();
        double coef = 1;
        while ('0' <= c && c <= '9')
            x += (c - '0') * (coef *= 1e-1), c = getChar();
    }
    return s == 1 ? x : -x;
}

inline void readWord(char *s) {
    int c = readChar();
    while (c > 32)
        *s++ = c, c = getChar();
    *s = 0;
}

inline bool readLine(char *s) {
    int c = getChar();
    while (c != '\n' && c != -1)
        *s++ = c, c = getChar();
    *s = 0;
    return c != -1;
}

// read() templates

template <typename T>
inline void read(T &) {}

template <>
inline void read(int &i)
{
    i = readInt();
}

template <>
inline void read(long long &i)
{
    i = readInt<long long>();
}

template <>
inline void read(unsigned int &i)
{
    i = readUInt();
}

template <>
inline void read(double &i)
{
    i = readDouble();
}

template <>
inline void read(char &i)
{
    i = readChar();
}

/** Write */

static int write_buf_pos = 0;
static char write_buf[buf_size];

inline void writeChar(int x) {
    if (write_buf_pos == buf_size)
        fwrite(write_buf, 1, buf_size, out_source), write_buf_pos = 0;
    write_buf[write_buf_pos++] = x;
}

inline void flush() {
    if (write_buf_pos) {
        fwrite(write_buf, 1, write_buf_pos, out_source), write_buf_pos = 0;
        fflush(out_source);
    }
}

template<class T>
inline void writeInt(T x, char end, int output_len) {
    if (x < 0)
        writeChar('-'), x = -x;

    char s[24];
    int n = 0;
    while (x || !n)
        s[n++] = '0' + x % 10, x /= 10;
    while (n < output_len)
        s[n++] = '0';
    while (n--)
        writeChar(s[n]);
    if (end)
        writeChar(end);
}

inline void writeWord(const char *s) {
    while (*s)
        writeChar(*s++);
}

inline void writeDouble(double x, int output_len) {
    if (x < 0)
        writeChar('-'), x = -x;
    assert(x <= (1LLU << 63) - 1);
    long long t = (long long) x;
    writeInt(t), x -= t;
    writeChar('.');
    for (int i = output_len - 1; i > 0; i--) {
        x *= 10;
        t = std::min(9, (int) x);
        writeChar('0' + t), x -= t;
    }
    x *= 10;
    t = std::min(9, (int) (x + 0.5));
    writeChar('0' + t);
}

#endif

int main() {
    // write your code here

    return 0;
}
