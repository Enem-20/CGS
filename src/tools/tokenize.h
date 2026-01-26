#ifndef N_TOKENIZE_H
#define N_TOKENIZE_H

#define TOKENIZE(x) #x

#define GENERATE_TYPE_FIELD(x)                          \
public:                                                 \
    inline const static QString _type = TOKENIZE(x);               


#endif // N_TOKENIZE_H