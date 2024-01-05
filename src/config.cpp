#include <iostream>
#include <fstream>
#include <string>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

string path;

void cfgSetPath(string newPath) {
  path = newPath;
}

int cfgGetResX() {
  
  Config cfg;

  try {
    cfg.readFile(path);
  }
  catch(const FileIOException &fioex) {
    std::cerr << "I/O error while reading file." << std::endl;
    exit(1);
  }
  catch(const ParseException &pex) {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    exit(1);
  }

  try
  {
    int sx = cfg.lookup("winsizx");
    return sx;
  }
  catch(const SettingNotFoundException &nfex)
  {
    cerr << "No 'winsizx' setting in configuration file." << endl;
  }

  return 1;

}

int cfgGetResY() {
  
  Config cfg;

  try {
    cfg.readFile(path);
  }
  catch(const FileIOException &fioex) {
    std::cerr << "I/O error while reading file." << std::endl;
    exit(1);
  }
  catch(const ParseException &pex) {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    exit(1);
  }

  try
  {
    int sy = cfg.lookup("winsizy");
    return sy;
  }
  catch(const SettingNotFoundException &nfex)
  {
    cerr << "No 'winsizy' setting in configuration file." << endl;
  }

  return 1;

}

string cfgGetTitle() {
  
  Config cfg;

  try {
    cfg.readFile(path);
  }
  catch(const FileIOException &fioex) {
    std::cerr << "I/O error while reading file." << std::endl;
    exit(1);
  }
  catch(const ParseException &pex) {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    exit(1);
  }

  try
  {
    string st = cfg.lookup("title");
    return st;
  }
  catch(const SettingNotFoundException &nfex)
  {
    cerr << "No 'name' setting in configuration file." << endl;
  }

  return "1";

}

