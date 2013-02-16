#ifndef D_FirFilterBuildTime_H
#define D_FirFilterBuildTime_H

///////////////////////////////////////////////////////////////////////////////
//
//  FirFilterBuildTime is responsible for recording and reporting when
//  this project library was built
//
///////////////////////////////////////////////////////////////////////////////

class FirFilterBuildTime
  {
  public:
    explicit FirFilterBuildTime();
    virtual ~FirFilterBuildTime();
    
    const char* GetDateTime();

  private:
      
    const char* dateTime;

    FirFilterBuildTime(const FirFilterBuildTime&);
    FirFilterBuildTime& operator=(const FirFilterBuildTime&);

  };

#endif  // D_FirFilterBuildTime_H
