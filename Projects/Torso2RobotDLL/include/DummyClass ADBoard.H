#ifndef ___DummyClass_ADBoard
#define ___DummyClass_ADBoard

///-----[Class_ADBoard]-------------------------------------------------
class Class_ADBoard {

  // Default constructor.
  Class_ADBoard(void);

  // Destructor.
  ~Class_ADBoard(void);

public:
  double ADIn(int ADChannel);
};


///-----[Class_DABoard]-------------------------------------------------
class Class_DABoard {

  // Default constructor.
  Class_DABoard(void);

  // Destructor.
  ~Class_DABoard(void);

public:
  int DAOut(int DAChannel, double Vout);
};


///-----[Class_CounterBoard]-------------------------------------------------
class Class_CounterBoard {

  // Default constructor.
  Class_CounterBoard(void);

  // Destructor.
  ~Class_CounterBoard(void);

public:
  int SetCount(int CNTChannel, int CountMedian);
  int Get(int CNTChannel);
};


#endif