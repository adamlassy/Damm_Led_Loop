#include <Arduino.h>
#include <Easing.h>

#define GLOBAL_GROW .25

class Beam
{

    int iSize;
    int iStartIndex;
    int iEndIndex;

    float fBrightStart;
    float fBrightEnd;

    float fLife;
    float fSpeed;
    float fGrow;

    //float fDecay;
    float fMaxBright;
    int iMaxSize;
    int bDirection;

    //boolean bAlive;
    boolean bMove;
 
    float fTargetDim;
    float fDim;
    //float fEasingIn, fEasingOut;

    int iDeath;

    int iDeathDay;
    
  public:

   Beam()
   {}
   
   Beam(int _size, float _speed, int _death, int _dir, int _index, float _grow, int _maxSize)
    {

      iSize = _size;
      fSpeed = _speed; 
      iMaxSize = _maxSize;

      bDirection = _dir;

      if (bDirection == 1) {
        iStartIndex = _index;
        iEndIndex = (_index+_size) % _maxSize;
      } else {
        iEndIndex = _index;
        iStartIndex = (_index+_size) % _maxSize;
      }
     
      fMaxBright = 1;
      fBrightStart = 0;
      fBrightEnd = 0;
      
      bMove = true;

      fDim = 0;
      fLife = 0;

      iDeath = _death;
      iDeathDay = .5 * _death;
      
      fGrow = _grow;
    }

    /*
     * Reset
     */
    void reset(int _size, float _speed, int _death, int _dir, int _index, float _grow, int _maxSize)
    {


    //Serial.println("SPEED : ");
    //Serial.println(_speed);


      iSize = _size;
      fSpeed = _speed; 
      //iStartIndex = _index;
      iMaxSize = _maxSize;

      //iEndIndex = (_index+_size) % _maxSize;
      bDirection = _dir;

      if (bDirection == 1) {
        iStartIndex = _index;
        iEndIndex = (_index+_size) % _maxSize;
      } else {
        iEndIndex = _index;
        iStartIndex = (_index+_size) % _maxSize;
      }

      Serial.println("OUT");
      Serial.println(iStartIndex);
      Serial.println(iEndIndex);
      
      fMaxBright = 1;
      fBrightStart = 0;
      fBrightEnd = 0;
      
      bMove = true;

      fDim = 0;
      fLife = 0;   

      iDeath = _death;
      iDeathDay = .25 * _death;
      fGrow = _grow;
    }

    int move()
    {
      
      if (bMove)
      {

        fBrightStart += fSpeed;
        fBrightEnd += (fSpeed + fGrow);

        if (fBrightStart >= fMaxBright) {

          iStartIndex = iStartIndex + bDirection;
          if (iStartIndex < 0) iStartIndex = iMaxSize - 1;
 
          iStartIndex = iStartIndex % iMaxSize;
          fBrightStart -= fMaxBright;
        }

        if (fBrightEnd >= fMaxBright) {

          iEndIndex = iEndIndex + bDirection;
          if (iEndIndex < 0) iEndIndex = iMaxSize - 1;

          iEndIndex = iEndIndex % iMaxSize;
          fBrightEnd -= fMaxBright;
        }
      }

      fLife += 1;
      fDim = Easing::easeInOutQuad(fLife, 0, 2.0, iDeath);
      if (fDim > 1.0) {
        fDim = 2.0 - fDim;
      }

      if (fLife >= iDeath) {
          return 0;
      }

      //Serial.println(fLife);
      //Serial.println(fDim);

      return 1;

    }

    int moveBothWays()
    {

      if (bMove)
      {

        fBrightStart += fSpeed * bDirection;
        fBrightEnd += (fSpeed + fGrow) * bDirection;

        if (bDirection == 1)
        {

          if (fBrightStart >= fMaxBright) {

            iStartIndex = iStartIndex + bDirection;
            if (iStartIndex < 0) iStartIndex = iMaxSize - 1;
 
            iStartIndex = iStartIndex % iMaxSize;
            fBrightStart -= fMaxBright;
          }

          if (fBrightEnd >= fMaxBright) {

            iEndIndex = iEndIndex + bDirection;
            if (iEndIndex < 0) iEndIndex = iMaxSize - 1;

            iEndIndex = iEndIndex % iMaxSize;
           fBrightEnd -= fMaxBright;
          }

        } else {

          //Serial.println(bDirection);
          
          if (fBrightStart <= 0) {

            iStartIndex = iStartIndex + bDirection;
            if (iStartIndex < 0) iStartIndex = iMaxSize - 1;
 
            //iStartIndex = iStartIndex % iMaxSize;
            fBrightStart += fMaxBright;
          }

          if (fBrightEnd <= 0) {

            iEndIndex = iEndIndex + bDirection;
            if (iEndIndex < 0) iEndIndex = iMaxSize - 1;

            //iEndIndex = iEndIndex % iMaxSize;
            fBrightEnd += fMaxBright;
          }

        }

      }

      fLife += 1;

      if (fLife < iDeathDay)
      {
        fDim = Easing::easeInSine(fLife, 0, 1.0, iDeathDay);
      } else {
        fDim = Easing::easeOutSine(fLife-iDeathDay, 0, 1.0, iDeath - iDeathDay);
        //fDim = Easing::easeOutSine(fLife, 0, 1.0, iDeath);
        fDim = 1 - fDim;
      }
      
      //if (fDim > 1.0) {
      //   fDim = 2.0 - fDim;
      //}

      if (fLife >= iDeath) {
          return 0;
      }
      
      //Serial.println(fLife);
      //Serial.println(fDim);
      
      return 1;
 
    }
    
    float getVal(int _index)
    {
      //Serial.println(_index);
      float _tBright;
 
      if (
        (iStartIndex < iEndIndex && (_index >= iStartIndex && _index <= iEndIndex))
        ||
        (iEndIndex < iStartIndex && (_index >= iStartIndex || _index <= iEndIndex))
      )
      {
        
        return fDim; //fBright * fDim;

      } else {

        return 0;
      }

    }
    
    float getValBright(int _index)
    {
      //Serial.println(_index);
      float _tBright;
 
      if (
        (
          bDirection == 1 && (
            (iStartIndex < iEndIndex && (_index >= iStartIndex && _index <= iEndIndex))
            ||
            (iEndIndex < iStartIndex && (_index >= iStartIndex || _index <= iEndIndex))
          )
        )
        ||
        (
          bDirection == -1 && (
            (iStartIndex > iEndIndex && (_index <= iStartIndex && _index >= iEndIndex))
            ||
            (iEndIndex > iStartIndex && (_index <= iStartIndex || _index >= iEndIndex))
          )
        )
      )
        
      {

        if (_index == iStartIndex) {

          _tBright = (bDirection > 0) ? 1-fBrightStart : fBrightStart;
        } else if (_index == iEndIndex) {

          _tBright = (bDirection > 0) ? fBrightEnd : 1-fBrightEnd;
        } else {
 
          _tBright = fMaxBright;
        }
        //_tBright = fMaxBright;
        
        return _tBright * fDim;

      } else {

        return 0;
      }

    }

};
