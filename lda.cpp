#include "lda.h"
#include "corpus.h"
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;
lda::lda(const corpus *c,double a,double b,double topic_num)
:alpha(a),beta(b),K(topic_num)
{
    setup(c);
    num_by_t = new int[K];
    num_by_d = new int[D];

    num_by_t_w = new int*[K];
    phi = new double*[K];
    for(int k=0;k<K;k++)
    {
	num_by_t_w[k] = new int[V];
	phi[k]=new double[V];
    }	

    num_by_d_t = new int*[D];
    theta = new double*[D];
    for(int d=0;d<D;d++)
    {
	num_by_d_t[d] = new int[K];
	theta[d] = new double[K];
    }

    p = new double[K];
    q = new double[K];
    srand((unsigned)time(0));
    
}

lda::~lda()
{
    
    delete [] num_by_t;
    delete [] num_by_d;

    for(int i=0;i<K;i++)
    {
	delete [] num_by_t_w[i];
	delete [] phi[i];
    }
    delete [] num_by_t_w;
    delete [] phi;

    for(int i=0;i<D;i++)
    {
	delete [] num_by_d_t[i];
	delete [] w[i];
	delete [] z[i];
	delete [] theta[i];
    }
    delete [] num_by_d_t;
    delete [] theta;

    delete [] p;
    delete [] q;
    delete [] l;
}


void lda::setup(const corpus *c)
{
    D = c->num_docs;
    V = c->size_vocab;
    l = new int[D];
    w = new int*[D];
    z = new int*[D];
    int word;
    int count;
    for(unsigned int d=0; d < D; d++)
    {
	document * doc = c->docs[d];
	w[d] = new int[doc->total];
	z[d] = new int[doc->total];
	l[d] = doc->total;
	int m=0;
	for(int n=0; n < doc->length; n++)
	{   
	    word = doc->words[n];
	    count = doc ->counts[n];
	    for(int j=0;j<count;j++)
	    {
		w[d][m]=word;
		z[d][m]=-1;
		m++;
	    }
	    //cout << l[d]<<" "<<m<<endl;
	}
	    assert(m==l[d]);
    }
    cout<<"setup finished!"<<endl;
}


void lda::init()
{
    int topic;
    cout<<"init"<<endl;
    int m,k,v,n;
    for(m=0;m<D;m++)
    {
	num_by_d[m]=0;
	for(k=0;k<K;k++)
	num_by_d_t[m][k]=0;
    }
    for(k=0;k<K;k++)
    {
	num_by_t[k]=0;
	for(v=0;v<V;v++)
	num_by_t_w[k][v]=0;
    }

    for(m=0;m<D;m++)
    {
	cout<<"doc:"<<m<<endl;
	for( n=0;n<l[m];n++)
	{
	    topic = (int)((rand()-1)/(double)(RAND_MAX/K));
	    z[m][n]=topic;
	//    cout<<"word:"<<n<<",topic:"<<topic<<endl;
	    num_by_t[topic]++;
	    num_by_d[m]++;
	    num_by_t_w[topic][w[m][n]]++;
	    num_by_d_t[m][topic]++;


	} 
    }
    cout<<"init finished"<<endl;
}

void lda::train(int iter_num)
{
    int topic=0;
    for(int i=0;i<iter_num;i++)
    {
	cout<<"iter:"<<i<<endl;
	for(int m=0;m<D;m++)
	{
	  //  cout<<"doc:"<<m<<endl;
	    for(int n=0;n<l[m];n++)
	    {
		topic = collapsed_sample_z(m,n);
		z[m][n]=topic;
//		cout<<"word:"<<n<<",topic:"<<topic<<endl;

	    } 
	}
    }
    update_parameter();
}

int lda::collapsed_sample_z(int m,int n)
{
    int old_t = z[m][n];
    int w_idx = w[m][n]; 
    int k=0; 
    num_by_t[old_t]--;
    num_by_d[m]--;
    num_by_t_w[old_t][w_idx]--;
    num_by_d_t[m][old_t]--;
    double total_p = 0.0;
    for(k=0;k<K;k++)
    {
	p[k]=(num_by_t_w[k][w_idx]+beta)*(num_by_d_t[m][k]+alpha)/(num_by_t[k]+V*beta);
	total_p+=p[k];
	q[k]=total_p;
    }
    double u = (rand()/(double)(RAND_MAX))*total_p;
    assert(u<=total_p&&u>=0);
    for(k=0;k<K;k++)
	if(u<q[k])break;

    num_by_t[k]++;
    num_by_d[m]++;
    num_by_t_w[k][w_idx]++;
    num_by_d_t[m][k]++;

    return k;
}

void lda::update_parameter()
{
    update_theta();
    update_phi();
}

void lda::update_theta()
{
    for(int m=0;m<D;m++)
	for(int k=0;k<K;k++)
	    theta[m][k]=(num_by_d_t[m][k]+alpha)/(num_by_d[m]+K*alpha);
}
void lda::update_phi()
{
    for(int k=0;k<K;k++)
	for(int v=0;v<V;v++)
	    phi[k][v]=(num_by_t_w[k][v]+beta)/(num_by_t[k]+V*beta);
}

void lda::print()
{
    for(unsigned int d=0; d < D; d++)
    {
	    cout<<"doc"<<d;
	for(int n=0; n < l[d]; n++)
	{   
//		cout<<" ;"<<w[d][n]<<" , "<<z[d][n];
	}
//	cout<<std::endl;
	cout<<"topic distribution:";
	for(int k=0;k<K;k++)
	    cout<<" : "<<theta[d][k];
	cout<<endl;
    }
    


}
