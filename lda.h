#ifndef __LDA__
#define __LDA__
//keep the same style with Chong Wang's HDP gibbs sampler code.
class corpus;
class lda
{
    public:
//fix parameters
    double alpha;
    double beta;
    int K;
    int V;
    int D;



    int *l;//doc length
    int **w;
//sampling state
    int **z;

    int *num_by_t; 
    int *num_by_d;

    int **num_by_t_w; 
    int **num_by_d_t; 

    double **theta;
    double **phi;

    
    double *p;
    double *q;
public:
    lda(const corpus *c,double alpha,double beta,double topic_num);
    virtual ~lda();
    
    void setup(const corpus *c);
    void init();
    void train(int iter_num);
    int collapsed_sample_z(int m,int n);
    void update_parameter();
    void update_theta();
    void update_phi();
    void print();

};
#endif
