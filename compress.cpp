#include<stdio.h>
#include<stdlib.h>
#include"IO.h"

HFnode  coding_data[256], dedata[256],clean= {0,0,0,NULL,NULL,NULL,NULL},* start ,startn= {-1,0,0,NULL,NULL,NULL,NULL},endn= {-1,0,0,NULL,NULL,NULL,NULL};
hash_node decode_table[256];
class Codec
{
public:

    void sort(HFnode * Ptr)
    {

        if (Ptr->next != &endn)
        {
            sort(Ptr->next);
        }
        else
        {
            return;
        }
        while (Ptr->next != &endn && Ptr->next->value < Ptr->value )
        {
            HFnode * tempsave ;
            tempsave = Ptr->next ;
            Ptr->previous->next=Ptr->next;
            Ptr->next->previous=Ptr->previous;
            Ptr->next = tempsave->next ;
            tempsave->next->previous=Ptr;
            tempsave->next=Ptr;
            Ptr->previous = tempsave;
        }
    }

    void HF_tree()
    {
        HFnode * myindex = &startn,*ntemp;
        while (startn.next->next != &endn)
        {
            ntemp = new HFnode();
            ntemp->NMcode= -1;
            ntemp->value=startn.next->value+startn.next->next->value;
            ntemp->SLeft = startn.next;
            ntemp->Sright = startn.next->next;
            ntemp->previous = &startn;
            ntemp->next=startn.next->next->next;
            startn.next->next->next->previous=ntemp;
            startn.next=ntemp;
            sort(startn.next);
            myindex = &startn;
        }
        startn.next->HFcode=1;
    }

    void encode(HFnode * currentnode)
    {
        if (currentnode->SLeft != NULL)
        {
            currentnode->SLeft->HFcode=currentnode->HFcode*2;
            encode( currentnode->SLeft);
        }

        if (currentnode->Sright != NULL)
        {
            currentnode->Sright->HFcode=currentnode->HFcode*2+1;
            encode(currentnode->Sright);
        }
    }

    void decode(IO &A)
    {
        A.CPR();
        for (int t = 0 ; t < 256 ; t++)
        {
            if(dedata[t].HFcode > 1)
            {
                decode_table[dedata[t].HFcode % 256].hash(t,dedata[t].HFcode);
            }
        }
    }
};

int main()
{
    IO* MyIO;
    Codec * MyCodec;
    int choice=3;
    printf("請選擇:\n\n");
    printf("  (1)壓縮檔案\n\n");
    printf("  (2)解壓縮檔案\n\n");
    printf(" 選擇:");
    scanf("%d",&choice);
    if (choice == 1)
    {
        MyIO=new IO ;
        MyCodec=new Codec;
        if ( MyIO->NR() )
        {
            MyCodec->sort(startn.next);
            MyCodec->HF_tree();
            MyCodec->encode(startn.next);
            MyIO->CW();
        }
        delete MyCodec;
        delete MyIO;

    }
    else if(choice == 2)
    {
        MyIO=new IO ;
        MyCodec=new Codec;
        MyCodec->decode(*MyIO);
        MyIO->NW();
        delete MyCodec;
        delete MyIO;
    }
    return 0 ;
}
