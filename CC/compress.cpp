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
HFnode  coding_data[256], dedata[256],clean={0,0,0,NULL,NULL,NULL,NULL},* start ,startn={-1,0,0,NULL,NULL,NULL,NULL},endn={-1,0,0,NULL,NULL,NULL,NULL};
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

}decode_table[256];

class IO
{
//	friend class codec;
private:

        int total[256];
        char b[20];
        unsigned char temp ,xorT,ptt ;
		FILE * source ;
		FILE * destination;
		char filename[20];

public :

		IO()
		{
			//destination = fopen("ts.cmp","wb+");
			temp = 0;
			xorT = 0;
			ptt=0;
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
							//////////
							//xorT = temp^ptt;
							//ptt = temp;
                            total[temp]++;
						//	printf("%d | ",xorT);
							//////////
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
		/////
		int CW()
		{
			source = fopen(b,"rb+");
			bool tsa[32]={0};
			int said=0;
			unsigned char df=0;
			int tn;
			printf("Please name the compression file. (*.cmp)\n");
			scanf("%s",filename);
			destination = fopen(filename,"wb");
			fwrite(coding_data,sizeof(coding_data),1,destination);
			//xorT=0;
			//ptt=0;
			while(fread(&temp,1,1,source))
			{
				//xorT = temp^ptt;
				//ptt = temp;
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

				//printf("\n");
			}
			fwrite(&df,1,1,destination);

			return 1;
		}
		/////

		bool bitwriter(bool mybit,unsigned char * contain)
		{
			//printf("%d",mybit);
			char bitmask[8]={1,2,4,8,16,32,64,128};
			static int counter;

			//printf("%d\n",counter);
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
		/////////
		void CPR()
		{
			printf("Please enter file wanting to compress. (*.cmp)\n");
			scanf("%s",filename);
			source = fopen(filename,"rb");

			source = fopen(filename,"rb");
			fread(dedata,sizeof(coding_data),1,source);
		}

		///////
		void NW()
		{
			unsigned char cnt;
			int tn=1,wc=0, ttime=0;

			printf("Please name file name after decompress. (*.cmp)\n");
			scanf("%s",filename);
			destination = fopen(filename,"wb");
			fseek ( source, sizeof(coding_data) , SEEK_SET);
			ptt=0;
			while(fread(&cnt,1,1,source))
			{
				ttime=0;
				//printf("%d-----\n",cnt);
				while(ttime < 8 )
				{
					tn=tn * 2 +( cnt %2 );
				//	printf("%d#####\n",tn);
					cnt = cnt / 2;
				//	printf("%d^^^^^^^\n",cnt);
					if(-1 != (wc=decode_table[tn% 256 ].show(tn)))
					{
				//		printf("%d\n",tn);
						xorT = wc^ptt;
						ptt = xorT;
						fwrite(&xorT,1,1,destination);
						wc=0;
						tn=1;
					}
					ttime++;
				}
			}

		}
/*
		///////*/
		~IO()
		{
			fclose(destination);
			fclose(source);
		}

}MyIO;

class codec
{
public:

        void sort(HFnode * Ptr)
        {

                if (Ptr->next != &endn)
                {
                        //printf("%d\n",Ptr->NMcode);
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
                        //printf("%d <-> %d " ,Ptr->value,tempsave->value);
                }
                //printf("\n");
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
				/*while (myindex != &endn)
                {
                        //printf("%d ",myindex->value );
                        myindex=myindex->next;
                }
				//printf("\n");*/
			}
			startn.next->HFcode=1;
        }

		void encode(HFnode * currentnode)
		{
			//printf("%d ",currentnode->value );
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

		void decode()
		{
			MyIO.CPR();

			for (int t = 0 ; t < 256 ; t++)
			{
				if(dedata[t].HFcode > 1)
				{
					decode_table[dedata[t].HFcode % 256].hash(t,dedata[t].HFcode);
				}
			}

/*
			for ( t = 0 ; t < 256 ; t++)
			{

					printf("%d\n",decode_table[dedata[t].HFcode % 256].show(dedata[t].HFcode));

			}
*/
			//printf("%d\n",decode_table[dedata[39].HFcode % 256].show(39));



		}




}MyCodec;

int main()
{
	int choice=3;
	printf(" 請選擇功能:\n\n");
	printf("  (1)壓縮檔案\n\n");
	printf("  (2)解壓縮檔案\n\n");
	printf(" 選擇:");
	scanf("%d",&choice);

	HFnode * id=&startn;

	if (choice == 1)
	{

        if ( MyIO.NR() )
        {

                MyCodec.sort(startn.next);

				MyCodec.HF_tree();
				MyCodec.encode(startn.next);

			for ( int t = 0 ; t < 256 ; t++)
			{

					printf("%d: %d --> %d\n",coding_data[t].NMcode,coding_data[t].value,coding_data[t].HFcode);

			}

				printf("\n");



				MyIO.CW();

        }
	}
	else if(choice == 2)
	{


		MyCodec.decode();



		MyIO.NW();
	}
	return 0 ;

}
