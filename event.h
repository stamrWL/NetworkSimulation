

class Event
{
private:
    double startTime;
    int fromType;
    int fromIndex;
    int ToType;
    int ToIndex;
public:
    Event(double startTime,double TaskSize,int fromType,int fromIndex,int ToType,int ToIndex){
        this->startTime = startTime;
        this->fromType = fromType;
        this->fromIndex = fromIndex;
        this->ToType = ToType;
        this->ToIndex = ToIndex;
    }
    int getFromType(){return fromType;}
    int getFromIndex(){return fromIndex;}
    int getToType(){return ToType;}
    int getToIndex(){return ToIndex;}
}