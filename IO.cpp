#include<stdio.h>
#include<stdlib.h>
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
    hash_node()
    {
        Nvalue = -1;
        HFvalue = 0;
        next = NULL;
        ctp = this;
    }
    void hash(int vl,int hf)
    {
        if (next == NULL)
        {
            Nvalue = vl;
            HFvalue = hf ;
            next = new hash_node();
            ctp = next;
        }
        else
        {
            while (ctp->next != NULL)
            {
                ctp=ctp->next;

            }
            ctp->Nvalue = vl;
            ctp->HFvalue = hf ;
            ctp->next = new hash_node();
        }
    }
    int show(int ch)
    {

        ctp = this;

        while (ctp->HFvalue  != ch)
        {
            if (ctp->next == NULL)
            {
                return -1;
            }
            ctp=ctp->next;

        }

        return ctp->Nvalue;
    }

};
extern HFnode  coding_data[256], dedata[256],clean= {0,0,0,NULL,NULL,NULL,NULL},* start ,startn= {-1,0,0,NULL,NULL,NULL,NULL},endn= {-1,0,0,NULL,NULL,NULL,NULL};
extern hash_node decode_table[256];
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

    IO()
    {
        temp = 0;
    }
    int NR()
    {
        printf("Please enter the file name that wants to compress.\n");
        scanf("%s",b);
        source = fopen(b,"rb+") ;
        int sflag=0,pn=0;
        if (source > 0)
        {
            while(fread(&temp,1,1,source))
            {
                total[temp]++;
            }
            fclose(source);
            coding_data[0]=clean;
            for (int t = 0 ; t < 256 ; t++)
            {
                coding_data[t]=clean;
                coding_data[t].value = total[t] ;
                coding_data[t].NMcode = t;
                if (coding_data[t].value <= 0)
                    continue;
                if (sflag == 0 )
                {
                    start = coding_data+t;
                    pn=t;
                    sflag = 1;
                    coding_data[t].previous=&startn;
                    startn.next=coding_data+t;
                    continue;
                }

                coding_data[pn].next=coding_data+t;
                coding_data[t].previous=coding_data+pn;
                pn=t;
                coding_data[t].next=&endn;
                endn.previous=coding_data+t;

            }
            return 1;
        }
        else
        {
            printf("Failure to read file!\n");
            return 0;
        }
    }
    int CW()
    {
        source = fopen(b,"rb+");
        bool tsa[32]= {0};
        int said=0;
        unsigned char df=0;
        int tn;
        printf("Please name the compression file. (*.cmp)\n");
        scanf("%s",filename);
        destination = fopen(filename,"wb");
        fwrite(coding_data,sizeof(coding_data),1,destination);
        while(fread(&temp,1,1,source))
        {
            tn=coding_data[temp].HFcode;
            said=0;

            while(tn > 1)
            {
                tsa[said++]=tn % 2;
                tn = tn / 2;
            }
            said--;
            while(said >= 0)
            {
                bitwriter(tsa[said--],&df);
            }
        }
        fwrite(&df,1,1,destination);
        return 1;
    }
    bool bitwriter(bool mybit,unsigned char * contain)
    {
        char bitmask[8]= {1,2,4,8,16,32,64,128};
        static int counter;
        if (mybit == 1)
        {
            *contain = *contain | bitmask[counter]	;
        }
        counter++;
        if (counter == 8)
        {
            fwrite(contain,1,1,destination);
            counter= 0;
            //printf("%d\n",*contain);
            *contain=0;

        }
        return 1;
    }
    void CPR()
    {
        printf("Please enter file wanting to compress. (*.cmp)\n");
        scanf("%s",filename);
        source = fopen(filename,"rb");

        source = fopen(filename,"rb");
        fread(dedata,sizeof(coding_data),1,source);
    }
    void NW()
    {
        unsigned char cnt;
        int tn=1,wc=0, ttime=0;
        printf("Please name file name after decompress. (*.cmp)\n");
        scanf("%s",filename);
        destination = fopen(filename,"wb");
        fseek ( source, sizeof(coding_data) , SEEK_SET);
        while(fread(&cnt,1,1,source))
        {
            ttime=0;
            while(ttime < 8 )
            {
                tn=tn * 2 +( cnt %2 );
                cnt = cnt / 2;
                if(-1 != (wc=decode_table[tn% 256 ].show(tn)))
                {
                    fwrite(&wc,1,1,destination);
                    wc=0;
                    tn=1;
                }
                ttime++;
            }
        }

    }
    ~IO()
    {
        fclose(destination);
        fclose(source);
    }

};
