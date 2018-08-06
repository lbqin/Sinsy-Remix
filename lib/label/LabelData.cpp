/* ----------------------------------------------------------------- */
/*           The HMM-Based Singing Voice Synthesis System "Sinsy"    */
/*           developed by Sinsy Working Group                        */
/*           http://sinsy.sourceforge.net/                           */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2015  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the Sinsy working group nor the names of    */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include "util_log.h"
#include "LabelData.h"

namespace sinsy
{
const INT64 LabelData::INVALID_TIME = -1;

namespace
{
const size_t NUM_P = 16;
const size_t NUM_A = 5;
const size_t NUM_B = 5;
const size_t NUM_C = 5;
const size_t NUM_D = 9;
const size_t NUM_E = 60;
const size_t NUM_F = 9;
const size_t NUM_G = 2;
const size_t NUM_H = 2;
const size_t NUM_I = 2;
const size_t NUM_J = 3;
const char* SEPARATOR_P[NUM_P] = {"", "@", "^", "-", "+", "=", "_", "%", "^", "_", "~", "-", "!", "[", "$", "]"};
const char* SEPARATOR_A[NUM_A] = {"/A:", "-", "-", "@", "~"};
const char* SEPARATOR_B[NUM_B] = {"/B:", "_", "_", "@", "|"};
const char* SEPARATOR_C[NUM_C] = {"/C:", "+", "+", "@", "&"};
const char* SEPARATOR_D[NUM_D] = {"/D:", "!", "#", "$", "%", "|", "&", ";", "-"};
const char* SEPARATOR_E[NUM_E] = {
   "/E:", "]", "^", "=", "~", "!", "@", "#", "+", "]",
   "$", "|", "[", "&", "]", "=", "^", "~", "#", "_",
   ";", "$", "&", "%", "[", "|", "]", "-", "^", "+",
   "~", "=", "@", "$", "!", "%", "#", "|", "|", "-",
   "&", "&", "+", "[", ";", "]", ";", "~", "~", "^",
   "^", "@", "[", "#", "=", "!", "~", "+", "!", "^"
};
const char* SEPARATOR_F[NUM_F] = {"/F:", "#", "#", "-", "$", "$", "+", "%", ";"};
const char* SEPARATOR_G[NUM_G] = {"/G:", "_"};
const char* SEPARATOR_H[NUM_H] = {"/H:", "_"};
const char* SEPARATOR_I[NUM_I] = {"/I:", "_"};
const char* SEPARATOR_J[NUM_J] = {"/J:", "~", "@"};

    const size_t PHONLEN = 6;
    const size_t PITCHLEN = 13;
    const size_t BEATLEN = 10;
    const std::string zhConVowel[61] = {"0","pau","b","c","ch","d","f","g","h","j","k","l","m","n","p","q","r","s","sh","t","w","x","y","z","zh",
                                      "a","i","v","u","o","e","en","ai","an","ia","ua","er","ii","uo","un","ui","iii","ue","in","ou",
                                      "ei","ao","iu","ie","van","ian","iao","ang","ing","uan","eng","ong","uai","uang","iang","iong"};
    const std::string phonemelan[PHONLEN] = {"0","s","p","v","c","b"};
    const std::string pithValue[PITCHLEN] = {"0","C","Db","D","Eb","E","F","Gb","G","Ab","A","Bb","b"};
    const std::string absPitch[121] = {"0","C0","C1","C2","C3","C4","C5","C6","C7","C8","C9",
                                   "Db0","Db1","Db2","Db3","Db4","Db5","Db6","Db7","Db8","Db9",
                                   "D0","D1","D2","D3","D4","D5","D6","D7","D8","D9",
                                   "Eb0","Eb1","Eb2","Eb3","Eb4","Eb5","Eb6","Eb7","Eb8","Eb9",
                                   "E0","E1","E2","E3","E4","E5","E6","E7","E8","E9",
                                   "F0","F1","F2","F3","F4","F5","F6","F7","F8","F9",
                                   "G0","G1","G2","G3","G4","G5","G6","G7","G8","G9",
                                   "Gb0","Gb1","Gb2","Gb3","Gb4","Gb5","Gb6","Gb7","Gb8","Gb9",
                                   "B0","B1","B2","B3","B4","B5","B6","B7","B8","B9",
                                   "Bb0","Bb1","Bb2","Bb3","Bb4","Bb5","Bb6","Bb7","Bb8","Bb9",
                                   "A0","A1","A2","A3","A4","A5","A6","A7","A8","A9",
                                   "Ab0","Ab1","Ab2","Ab3","Ab4","Ab5","Ab6","Ab7","Ab8","Ab9"
    };
    const std::string beatype[BEATLEN]={"0","1/4","2/4","3/4","4/4","3/8","6/8","7/8","9/8","12/8"};
    const std::string scoreflag[10] = {"0","00","80", "40", "20", "10", "08", "04", "02", "01"};
    const std::string relapitch[12] = {"0","1","2", "3", "4", "5", "6", "7", "8", "9","10","11"};
    const std::string pitchdiff[24] = {"0","m1","m2","m3","m4","m5","m6","m7","m8","m9","m10","m11",
                                "p0","p1","p2","p3","p4","p5","p6","p7","p8","p9","p10","p11"};
    const std::string dynamics[20] = {"0","p4","p3","p2","p1","mp","n","mf","f1","f2","f3","f4",
                                 "pppp","ppp","pp","p","f","ff","fff","ffff"};

};

/*!
 constructor
 */
LabelData::LabelData() : monophoneFlag(false), outputTimeFlag(true), beginTime(INVALID_TIME), endTime(INVALID_TIME)
{
   List *p(NULL), *a(NULL), *b(NULL), *c(NULL), *d(NULL), *e(NULL), *f(NULL), *g(NULL), *h(NULL), *i(NULL), *j(NULL);

   try {
      p = new List(NUM_P, "0");
      a = new List(NUM_A, "0");
      b = new List(NUM_B, "0");
      c = new List(NUM_C, "0");
      d = new List(NUM_D, "0");
      e = new List(NUM_E, "0");
      f = new List(NUM_F, "0");
      g = new List(NUM_G, "0");
      h = new List(NUM_H, "0");
      i = new List(NUM_I, "0");
      j = new List(NUM_J, "0");
   } catch (const std::bad_alloc&) {
      delete p;
      delete a;
      delete b;
      delete c;
      delete d;
      delete e;
      delete f;
      delete g;
      delete h;
      delete i;
      delete j;
      ERR_MSG("Cannot allocate memory");
   }

   data.insert(std::make_pair('p', p));
   data.insert(std::make_pair('a', a));
   data.insert(std::make_pair('b', b));
   data.insert(std::make_pair('c', c));
   data.insert(std::make_pair('d', d));
   data.insert(std::make_pair('e', e));
   data.insert(std::make_pair('f', f));
   data.insert(std::make_pair('g', g));
   data.insert(std::make_pair('h', h));
   data.insert(std::make_pair('i', i));
   data.insert(std::make_pair('j', j));
}

/*!
 destructor
 */
LabelData::~LabelData()
{
   Data::iterator itr(data.begin());
   Data::iterator itrEnd(data.end());
   for (; itr != itrEnd; ++itr) {
      delete itr->second;
   }
}

/*!
 set monophone flag
 */
void LabelData::setMonophoneFlag(bool b)
{
   monophoneFlag = b;
}

/*!
 set output flag
 */
void LabelData::setOutputTimeFlag(bool b)
{
   outputTimeFlag = b;
}

/*!
 set begin time
 */
void LabelData::setBeginTime(double d)
{
   beginTime = static_cast<INT64>(d * 1.0e+7);
}

/*!
 set end time
 */
void LabelData::setEndTime(double d)
{
   endTime = static_cast<INT64>(d * 1.0e+7);
}

/*!
 get data
 */
const std::string& LabelData::get(char category, size_t number) const
{
   Data::const_iterator itr(data.find(category));
   if (data.end() == itr) {
      throw std::runtime_error("LabelData::get() unknown category");
   }
   const List* list(itr->second);
   if ((0 == number) || (list->size() < number)) {
      throw std::runtime_error("LabelData::get() number is out of range");
   }
   return list->at(number - 1);
}

/*!
 set data (std::string)
 */
template<>
void LabelData::set<std::string>(char category, size_t number, const std::string& value)
{
   Data::iterator itr(data.find(category));
   if (data.end() == itr) {
      throw std::runtime_error("LabelData::set() unknown category");
   }
   List* list(itr->second);
   if ((0 == number) || (list->size() < number)) {
      throw std::runtime_error("LabelData::set() number is out of range");
   }
   list->at(number - 1) = value;
}

/*!
 set data (bool)
 */
template<>
void LabelData::set<bool>(char category, size_t number, const bool& value)
{
   set(category, number, std::string(value ? "1" : "0"));
}
    std::string computeOnehotFeature(const std::vector<std::string>& target,const std::string& idstr){
       std::string features;
       if(target[0]==idstr){
          features.push_back('1');
       }
       else
          features.push_back('0');
       for (int i = 1; i < target.size(); ++i) {
          features.push_back(' ');
          if(target[i] == idstr){
             features.push_back('1');
          } else{
             features.push_back('0');
          }
       }
       return features;
    }
/*!
 to stream
 */
std::ostream& operator<<(std::ostream& os, const LabelData& obj)
{
   //if (obj.outputTimeFlag) {
      //os << obj.beginTime << " " << obj.endTime << " ";
   //}

   LabelData::Data::const_iterator itrP(obj.data.find('p'));
   if (obj.data.end() == itrP)
      throw std::runtime_error("LabelData::operator<<() p is not found");
   LabelData::List& p = *(itrP->second);
   LabelData::Data::const_iterator itrA(obj.data.find('a'));
   if (obj.data.end() == itrA)
      throw std::runtime_error("LabelData::operator<<() a is not found");
   LabelData::List& a = *(itrA->second);
   LabelData::Data::const_iterator itrB(obj.data.find('b'));
   if (obj.data.end() == itrB)
      throw std::runtime_error("LabelData::operator<<() b is not found");
   LabelData::List& b = *(itrB->second);
   LabelData::Data::const_iterator itrC(obj.data.find('c'));
   if (obj.data.end() == itrC)
      throw std::runtime_error("LabelData::operator<<() c is not found");
   LabelData::List& c = *(itrC->second);
   LabelData::Data::const_iterator itrD(obj.data.find('d'));
   if (obj.data.end() == itrD)
      throw std::runtime_error("LabelData::operator<<() d is not found");
   LabelData::List& d = *(itrD->second);
   LabelData::Data::const_iterator itrE(obj.data.find('e'));
   if (obj.data.end() == itrE)
      throw std::runtime_error("LabelData::operator<<() e is not found");
   LabelData::List& e = *(itrE->second);
   LabelData::Data::const_iterator itrF(obj.data.find('f'));
   if (obj.data.end() == itrF)
      throw std::runtime_error("LabelData::operator<<() f is not found");
   LabelData::List& f = *(itrF->second);
   LabelData::Data::const_iterator itrG(obj.data.find('g'));
   if (obj.data.end() == itrG)
      throw std::runtime_error("LabelData::operator<<() g is not found");
   LabelData::List& g = *(itrG->second);
   LabelData::Data::const_iterator itrH(obj.data.find('h'));
   if (obj.data.end() == itrH)
      throw std::runtime_error("LabelData::operator<<() h is not found");
   LabelData::List& h = *(itrH->second);
   LabelData::Data::const_iterator itrI(obj.data.find('i'));
   if (obj.data.end() == itrI)
      throw std::runtime_error("LabelData::operator<<() i is not found");
   LabelData::List& i = *(itrI->second);
   LabelData::Data::const_iterator itrJ(obj.data.find('j'));
   if (obj.data.end() == itrJ)
      throw std::runtime_error("LabelData::operator<<() j is not found");
   LabelData::List& j = *(itrJ->second);

   if (obj.monophoneFlag) {
      os << p[3];
      return os;
   }

   // p
   for (size_t idx(0); idx < NUM_P; ++idx) {
     // os << SEPARATOR_P[idx] << p[idx];
      if(idx==0){
         std::vector<std::string> phonlanValues;
         phonlanValues.assign(phonemelan,phonemelan+PHONLEN);
         os << computeOnehotFeature(phonlanValues,p[idx])<< " ";
      }
      else if(idx>0&&idx<6){
         std::vector<std::string> conVowelValues;
         conVowelValues.assign(zhConVowel,zhConVowel+61);
         os << computeOnehotFeature(conVowelValues,p[idx])<< " ";
      }
      else if(idx>5&&idx<11){
         std::vector<std::string> scoreflagValues;
         scoreflagValues.assign(scoreflag,scoreflag+10);
         os << computeOnehotFeature(scoreflagValues,p[idx])<< " ";
      }
      else
         os << p[idx]<< " ";
   }

   // a
   for (size_t idx(0); idx < NUM_A; ++idx) {
      //os << SEPARATOR_A[idx] << a[idx];
      os <<  a[idx]<< " ";
   }

   // b
   for (size_t idx(0); idx < NUM_B; ++idx) {
      //os << SEPARATOR_B[idx] << b[idx];
      os << b[idx]<< " ";
   }

   // c
   for (size_t idx(0); idx < NUM_C; ++idx) {
      //os << SEPARATOR_C[idx] << c[idx];
      os << c[idx]<< " ";
   }

   // d
   for (size_t idx(0); idx < NUM_D; ++idx) {
     // os << SEPARATOR_D[idx] << d[idx];
      if(idx==0){
         std::vector<std::string> absPitchValues;
         absPitchValues.assign(absPitch,absPitch+121);
         os << computeOnehotFeature(absPitchValues,d[idx])<< " ";
      }else if(idx == 1){
         std::vector<std::string> relapitchValues;
         relapitchValues.assign(relapitch,relapitch+12);
         os << computeOnehotFeature(relapitchValues,d[idx])<< " ";
      }else if(idx == 3){
         std::vector<std::string> beatypeValues;
         beatypeValues.assign(beatype,beatype+BEATLEN);
         os << computeOnehotFeature(beatypeValues,d[idx])<< " ";
      }else
         os << d[idx]<< " ";
   }

   // e
   for (size_t idx(0); idx < NUM_E; ++idx) {
      //os << SEPARATOR_E[idx] << e[idx];
      if(idx==0){
          std::vector<std::string> absPitchValues;
          absPitchValues.assign(absPitch,absPitch+121);
         os << computeOnehotFeature(absPitchValues,e[idx])<< " ";
      }else if(idx == 1){
          std::vector<std::string> relapitchValues;
          relapitchValues.assign(relapitch,relapitch+12);
         os << computeOnehotFeature(relapitchValues,e[idx])<< " ";
      }else if(idx == 3){
          std::vector<std::string> beatypeValues;
          beatypeValues.assign(beatype,beatype+BEATLEN);
         os << computeOnehotFeature(beatypeValues,e[idx])<< " ";
      }
      else if(idx == 27){
          std::vector<std::string> dynamicsValues;
          dynamicsValues.assign(dynamics,dynamics+20);
         os << computeOnehotFeature(dynamicsValues,e[idx])<< " ";
      }
      else if(idx > 27&& idx <56){

      }
      else if(idx == 56||idx == 57){
          std::vector<std::string> pitchdiffValues;
          pitchdiffValues.assign(pitchdiff,pitchdiff+24);
         os << computeOnehotFeature(pitchdiffValues,e[idx])<< " ";
      }
      else
         os << e[idx]<< " ";
   }

   // f
   for (size_t idx(0); idx < NUM_F; ++idx) {
      //os << SEPARATOR_F[idx] << f[idx];
       if(idx==0){
           std::vector<std::string> absPitchValues;
           absPitchValues.assign(absPitch,absPitch+121);
           os << computeOnehotFeature(absPitchValues,f[idx])<< " ";
       }else if(idx == 1){
           std::vector<std::string> relapitchValues;
           relapitchValues.assign(relapitch,relapitch+12);
           os << computeOnehotFeature(relapitchValues,f[idx])<< " ";
       }else if(idx == 3){
           std::vector<std::string> beatypeValues;
           beatypeValues.assign(beatype,beatype+BEATLEN);
           os << computeOnehotFeature(beatypeValues,f[idx])<< " ";
       }else
         os << f[idx]<< " ";
   }

   // g
   for (size_t idx(0); idx < NUM_G; ++idx) {
      //os << SEPARATOR_G[idx] << g[idx];
      os << g[idx]<< " ";
   }

   // h
   for (size_t idx(0); idx < NUM_H; ++idx) {
      //os << SEPARATOR_H[idx] << h[idx];
      os << h[idx]<< " ";
   }

   // i
   for (size_t idx(0); idx < NUM_I; ++idx) {
      //os << SEPARATOR_I[idx] << i[idx];
      os << i[idx]<< " ";
   }

   // j
   for (size_t idx(0); idx < NUM_J-1; ++idx) {
      //os << SEPARATOR_J[idx] << j[idx];
      os << j[idx]<< " ";
   }os << j[NUM_J-1];

   return os;
}


};  // namespace sinsy
