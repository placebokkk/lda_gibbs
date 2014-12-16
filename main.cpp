#include "lda.h"
#include "corpus.h"

int main()
{
    const char* data_path = "./tr_790.train";
    corpus* c = new corpus();
    c->read_data(data_path);
    lda *model = new lda(c,2,0.5,4);
    model->init();
    model->train(20);
    model->print();

}
