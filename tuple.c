typedef struct tuple{
  int size;  
  double score;
  int place;
  int averageWithin;
};

void* init_tuple(int size,double score,int place,int averageWithin){
  tuple *ret=(tuple *)malloc(sizeof(tuple));
  ret->size=size;ret->score=score;ret->place=place;ret->averageWithin=averageWithin;
  return ret;
}

int compare_to(const void *a,const void *b){
  tuple *x=const (tuple *)a;
  tuple *y=const (tuple *)b;
  if((x->size)>(y->size)){
      return 1;
  if((x->size)<(y->size))
    return -1;
  if((x->score)>(y->score)){
      return 1;
  if((x->score)<(y->score))
    return -1;
  return (x->place)-(y->place);  
}
