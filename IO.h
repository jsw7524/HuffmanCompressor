#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

struct HFnode
{
    unsigned int NMcode;
    unsigned int value;
    unsigned int HFcode;
    HFnode * SLeft;
    HFnode * Sright;
    HFnode * next;
    HFnode * previous;
};
class hash_node
{
private:
    int Nvalue;
    int HFvalue;
    hash_node * next;
    hash_node * ctp;
public:
    hash_node();
    void hash(int vl,int hf);
    int show(int ch);
};
class IO
{
    friend class codec;
private:
    int total[256];
    char b[20];
    unsigned char temp  ;
    FILE * source ;
    FILE * destination;
    char filename[20];
public :
    IO();
    int NR();
    int CW();
    bool bitwriter(bool mybit,unsigned char * contain);
    void CPR();
    void NW();
    ~IO();
};

#endif // IO_H_INCLUDED
