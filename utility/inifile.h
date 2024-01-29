#pragma once

#include<map>
using namespace std;
#include <string.h>
#include<sstream>
using std::ostringstream;

namespace web_rpc{
namespace utility{

class Value
{
public:
  Value();
  Value(bool value);
  Value(int value);
  Value(double value);
  Value(const string & value);
  ~Value();


  Value& operator = (int value);
  Value& operator = (bool value);
  Value& operator = (double value);
  Value& operator = (const string& value);

  operator bool();
  operator int();
  operator double();
  operator string();
  operator string() const;
  
private:
 string m_value;

};

class IniFile
{
private:
  IniFile();
  IniFile(IniFile&& other) = delete;
  IniFile& operator = (IniFile&& other) = delete;

  IniFile(const string& fileName);
  ~IniFile();
public:
  static IniFile& instance()
  {
    static IniFile inifile;
    return inifile;
  }
  bool load(const string& fileName);
  void save(const string &fileName);
  void show();
  void clear();
  // read values in different formats
  Value &get(const string &section, const string &key);

  // set values in different formats
  void set(const string &section, const string &key, bool value);
  void set(const string &section, const string &key, int value);
  void set(const string &section, const string &key, double value);
  void set(const string &section, const string &key, const string &value);

  bool has(const string &section);
  bool has(const string &section, const string &key);

  void remove(const string &section);
  void remove(const string &section, const string &key);

  typedef std::map<string, Value> Section;
  Section & operator [] (const string & key)
  {
      return m_inifile[key];
  }

private:
   string trim(string s);
   std::map<string,Section> m_inifile;
   string m_fileName;
};




}
}