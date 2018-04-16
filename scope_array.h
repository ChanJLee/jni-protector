#ifndef SCOPE_ARRAY
#define SCOPE_ARRAY

template <typename T>
class ScopeArray
{
  private:
    const T *mArr;

  public:
    ScopeArray(const T* arr): mArr(arr) {

    }

    virtual ~ScopeArray() {
        if (mArr) {
            delete[] mArr;
        }
    }
};

#endif