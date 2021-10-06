#ifndef _CPP_Playground_ORGANIZER_HPP_
#define _CPP_Playground_ORGANIZER_HPP_

#define MSTRING( L )  Stringize(L) 
#define Stringize( L )     #L 
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringize, __LINE__ )
#define PrintError __FILE__ "(" $Line ") : Error: "
#define PrintWarn  __FILE__ "(" $Line ") : Warning: "

#include <memory>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <thread>
#include <functional>
#include <type_traits>

#ifndef COMPILE_WITH_UNIMPLEMENTED_RUNNER
#define COMPILE_WITH_UNIMPLEMENTED_RUNNER 1
#endif // !COMPILE_WITH_UNIMPLEMENTED_RUNNER

#define START_ORGANIZER_MAIN_WITH_OBJS(...) \
int CppOrganizer::ICodeRunnerIdentifier::quest_id = 0; \
int main(int argc, char *argv[]) { \
  CppOrganizer::PlaygroundOrganizer po; \
  __VA_ARGS__ \
  po.PrintDetails(); \
  po.RunAll(); 
#define PAUSE_ON_END \
std::cout << "Press enter to exit programm.."; \
std::string s; \
std::getline(std::cin, s);
#define END_ORGANIZER_MAIN \
  return EXIT_SUCCESS; \
}

#define CREATE_ELEMENT(ClassName)       \
class ClassName :                       \
  public CppOrganizer::CodeRunnerHelper<ClassName> {  \
public:                                 \
  ClassName() {}                        \
  void Run();                           \
  constexpr const char* name() const  { \
    return MSTRING(ClassName); }        \
}

#define ELEMENT_CODE(ClassName) void ClassName::Run()

#define ADD_ELEMENT(ClassName, ...) \
po.builder().Add<ClassName>(__VA_ARGS__)
//std::make_shared<ClassName>(__VA_ARGS__)

#define CREATE_ELEMENT_WITH_CODE(ClassName)  \
CREATE_ELEMENT(ClassName);              \
ELEMENT_CODE(ClassName)

#include <assert.h>     /* assert */

#ifndef _ASSERT_EXPR
#define _ASSERT_EXPR(a,b) assert(a)
#endif

#ifndef _ASSERT
#define _ASSERT(a) assert(a)
#endif


namespace colorconsole {

#ifdef _WIN32
#include <Windows.h>
  class ConsoleColorManager {
  public:
    enum Color {
      _default = 0,
      fg_blue = FOREGROUND_BLUE,
      fg_green = FOREGROUND_GREEN,
      fg_red = FOREGROUND_RED,
      fg_intensified = FOREGROUND_INTENSITY,
      bg_blue = BACKGROUND_BLUE,
      bg_green = BACKGROUND_GREEN,
      bg_red = BACKGROUND_RED,
      bg_intensified = BACKGROUND_INTENSITY,
    };

    ConsoleColorManager()
    {
      hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hConsole == NULL/*nullptr*/) {
        return;
      }
      /* Save current attributes */
      if (!GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) return;
      saved_attributes = consoleInfo.wAttributes;
      bInitialized = true;
    }

    ~ConsoleColorManager() {
      /* Restore original attributes */
      if (hConsole)
        SetConsoleTextAttribute(hConsole, saved_attributes);
      //printf("Back to normal");
    }

    bool SetConsoleColor(unsigned short color = Color::_default) {
      if (!bInitialized) return false;
      return SetConsoleTextAttribute(hConsole, color) > 0;
    }

    void Default() {
      SetConsoleTextAttribute(hConsole, saved_attributes);
    }

    ConsoleColorManager(const ConsoleColorManager&) = delete;
    ConsoleColorManager& operator=(const ConsoleColorManager&) = delete;
  private:
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    HANDLE hConsole;
    bool bInitialized = false;
  };
#else
  class ConsoleColorManager {
    const char* RESET = "\033[0m";
    const char* BLACK = "\033[30m";      /* Black */
    const char* RED = "\033[31m";      /* Red */
    const char* GREEN = "\033[32m";      /* Green */
    const char* YELLOW = "\033[33m";      /* Yellow */
    const char* BLUE = "\033[34m";      /* Blue */
    const char* MAGENTA = "\033[35m";      /* Magenta */
    const char* CYAN = "\033[36m";      /* Cyan */
    const char* WHITE = "\033[37m";      /* White */
    const char* BOLDBLACK = "\033[1m\033[30m";      /* Bold Black */
    const char* BOLDRED = "\033[1m\033[31m";      /* Bold Red */
    const char* BOLDGREEN = "\033[1m\033[32m";      /* Bold Green */
    const char* BOLDYELLOW = "\033[1m\033[33m";      /* Bold Yellow */
    const char* BOLDBLUE = "\033[1m\033[34m";      /* Bold Blue */
    const char* BOLDMAGENTA = "\033[1m\033[35m";      /* Bold Magenta */
    const char* BOLDCYAN = "\033[1m\033[36m";      /* Bold Cyan */
    const char* BOLDWHITE = "\033[1m\033[37m";      /* Bold White */

  public:
    enum Color {
      _default = 0,
      fg_blue = 1,
      fg_green = 2,
      fg_red = 4,
      fg_intensified = 8,
    };
    bool SetConsoleColor(unsigned short color) {
      if ((color & fg_intensified))
      {
        if ((color & fg_blue))
        {
          puts(BLUE);
        }
        else if ((color & fg_green))
        {
          puts(GREEN);
        }
        else if ((color & fg_red))
        {
          puts(RED);
        }
        else
        {
          return false;
        }
      }
      else
      {
        if ((color & fg_blue))
        {
          puts(BOLDBLUE);
        }
        else if ((color & fg_green))
        {
          puts(BOLDGREEN);
        }
        else if ((color & fg_red))
        {
          puts(BOLDRED);
        }
        else
        {
          return false;
        }
      }
      return true;
    }
    void Default() {
      puts(RESET);
    }
  };
#endif

}

namespace CppOrganizer
{

  using colorconsole::ConsoleColorManager;
  class PlaygroundOrganizer;

  class ICodeRunnerIdentifier {
    friend class PlaygroundOrganizer;
  public:
    explicit ICodeRunnerIdentifier(const char* _name) :
      id(quest_id++), name(_name) { }

    std::string getName() const {
      return name;
    }

    int getID() const {
      return id;
    }

  private:
    virtual void RunCode() {};
    const std::string name;
    const int id;
    static int quest_id;
  };
  typedef std::shared_ptr<ICodeRunnerIdentifier> ICodeRunnerIdentifierPtr;

  class CodeExecuteBenchmark {
  public:
    template <typename Fun>
    CodeExecuteBenchmark(const Fun& func, const ICodeRunnerIdentifier& iden_)
    {
      using clock = std::chrono::high_resolution_clock;
      using duration = std::chrono::duration<double, std::milli>;
      ConsoleColorManager ccm;
      try
      {
        ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_green);
        printf("\n __START__ ");
        ccm.Default();
        printf("%d: ", iden_.getID());
        ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_blue);
        printf("%s", iden_.getName().c_str());
        ccm.Default();
        printf(" is starting execute\n");
        clock::time_point start = clock::now();
        func();
        duration elapsed = clock::now() - start;
        ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_red);
        printf("\n  __END__  ");
        ccm.Default();
        printf("%d: ", iden_.getID());
        ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_blue);
        printf("%s", iden_.getName().c_str());
        ccm.Default();
        printf(" completed in");
        ccm.SetConsoleColor(ConsoleColorManager::fg_intensified | ConsoleColorManager::fg_green);
        printf(" %.2f ", elapsed.count());
        ccm.Default();
        printf("msec\n");
      }
      catch (const std::exception& ex)
      {
        printf("\nCode[%d]:%s failed to execute!\nException message:\n%s", iden_.getID(), iden_.getName().c_str(), ex.what());
      }
      catch (...)
      {
        printf("\nCode[%d]:%s failed to execute!\nUnhandled Exception occured!\n", iden_.getID(), iden_.getName().c_str());
      }
    }
    ~CodeExecuteBenchmark() = default;
  private:
    //std::function<void()> f_;
    //const ICodeRunnerIdentifier& iden_;
  };

  template <class T>
  class CodeRunnerHelper : public ICodeRunnerIdentifier {
  public:
    // CodeRunnerHelper(const std::string& testName = typeid(T).name()) : ICodeRunnerIdentifier(testName) { }
    CodeRunnerHelper() :
      ICodeRunnerIdentifier(static_cast<T const*>(this)->name()) { }
    virtual ~CodeRunnerHelper() {};

    void RunCode() override {
      CodeExecuteBenchmark( std::bind(&T::Run, static_cast<T *>(this)), *this );
    }

  protected:
#if COMPILE_WITH_UNIMPLEMENTED_RUNNER == 1
    void Run() {
      printf("\n%s\n%s::Run() should be implemented!\n", PrintWarn, typeid(T).name());;
    }
#endif
  };

  struct CodeRunnerComp {
    using is_transparent = void; // for example with void,
                                 // but could be int or struct CanSearchOnId;
    bool operator()(ICodeRunnerIdentifierPtr const& lhs, ICodeRunnerIdentifierPtr const& rhs) const
    {
      return lhs->getID() < rhs->getID();
    }

    bool operator()(int id, ICodeRunnerIdentifierPtr const& rhs) const
    {
      return id < rhs->getID();
    }

    bool operator()(ICodeRunnerIdentifierPtr const& lhs, int id) const
    {
      return lhs->getID() < id;
    }
    //bool operator()(const std::string str, ICodeRunnerIdentifierPtr const& rhs) const
    //{
    //  return str.compare(rhs->getName()) < 0;
    //}

    //bool operator()(ICodeRunnerIdentifierPtr const& lhs, const std::string str) const
    //{
    //  return lhs->getName().compare(str) < 0;
    //}
  };
  typedef std::set<ICodeRunnerIdentifierPtr, CodeRunnerComp> IPlaygroundObjMap;

  // Add helper without using macro
  //template <class ClassName, typename ...Args>
  //std::shared_ptr<ClassName> Add(Args&&... args) {
  //  // ctor'da debug edebilmek için
  //  return std::shared_ptr<ClassName>(new ClassName(std::forward<Args>(args)...));
  //  //return std::make_shared<ClassName, Args...>(std::forward<Args>(args)...);
  //}

  class PlaygroundBuilder {
  public:
    PlaygroundBuilder() = default;
    ~PlaygroundBuilder() = default;

    template <class ClassName, typename ...Args>
    PlaygroundBuilder& Add(Args&&... args) {
      m_pg_objects.insert(std::shared_ptr<ClassName>(new ClassName(std::forward<Args>(args)...)));
      //m_pg_objects.insert(std::make_shared<ClassName, Args...>(std::forward<Args>(args)...));
      return *this;
    }
  private:
    friend class PlaygroundOrganizer;
    IPlaygroundObjMap m_pg_objects;
  };

  class PlaygroundOrganizer final {
  public:
    PlaygroundOrganizer() = default;

    ~PlaygroundOrganizer() = default;

    PlaygroundBuilder& builder() {
      return _builder;
    }

    void RunWithID(std::initializer_list<int> pg_ids)
    {
      for (auto id : pg_ids) {
        auto it = _builder.m_pg_objects.find(id);
        if (it != _builder.m_pg_objects.end())
        {
          (*it)->RunCode();
        }
      }
    }

    void RunWithName(std::initializer_list<std::string> pg_names)
    {
      for (auto name : pg_names) {
        auto it = findIDbyName(name);
        if (it != _builder.m_pg_objects.end())
        {
          (*it)->RunCode();
        }
      }
    }

    void RunAll()
    {
      std::for_each(
        _builder.m_pg_objects.begin(),
        _builder.m_pg_objects.end(),
        [](auto const& pg) { pg->RunCode(); });
    }

    std::string GetQuestionName(int id) {
      auto it = _builder.m_pg_objects.find(id);
      return it != _builder.m_pg_objects.end() ? (*it)->name : std::string();
    }

    int GetQuestionIDbyName(const std::string& name) {
      auto iter = findIDbyName(name);
      return iter != _builder.m_pg_objects.end() ? (*iter)->id : -1; // or return directly first as id
    }

    void PrintDetails() {
      printf("available items:");
      std::for_each(_builder.m_pg_objects.begin(), _builder.m_pg_objects.end(), [](auto const& q) {
        printf("\n%02d-name: %s", q->id, q->name.c_str());
      });
    }

  private:
    inline IPlaygroundObjMap::const_iterator findIDbyName(const std::string& name) {
      auto res = std::find_if(_builder.m_pg_objects.begin(), _builder.m_pg_objects.end(), [name](const ICodeRunnerIdentifierPtr& item) {
        return item->getName().compare(name) == 0;
      });
      return res;
    }
    PlaygroundBuilder _builder;
  };

}

#endif // _CPP_Playground_ORGANIZER_HPP_
