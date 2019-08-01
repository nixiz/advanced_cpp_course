#include <playground_organizer.hpp>
#include <iostream>
#include <string>

// Inheritance
#include "friend_usage.hpp"
#include "inheritance_usage.hpp"
#include "diamond_problem.hpp"
#include "virtual_destructor.hpp"

// Smart Pointers
#include "malloc_usage.hpp"
#include "smart_pointer.hpp"
#include "shared_pointer.hpp"
#include "weak_pointer.hpp"
#include "bonus/bonus_old_style_smart_pointer.hpp"

// Static Usage
#include "static_usage.hpp"

// CPU Architecture
#include "cpu_cache_misses.hpp"
#include "branch_prediction.hpp"
#include "packed_data_access.hpp"
#include "false_sharing.hpp"

// Templates
#include "template_mixin.hpp"
#include "template_staticpolymorphism.hpp"
#include "template_crtp_usage.hpp"
#include "template_decltype.hpp"
#include "template_container.hpp"
#include "template_policies.hpp"
#include "template_static_assert.hpp"
#include "template_inheritance_usage.hpp"

// Exceptions
#include "exception_usage.hpp"

// Custom Comparators
#include "custom_comparator.hpp"

// C++ 11 Features
#include "async_usage.hpp"


int CppOrganizer::ICodeRunnerIdentifier::quest_id = 0;
int main(int argc, char *argv[]) {
    CppOrganizer::PlaygroundOrganizer po;
    po.builder()
      // 2- inheritance
      .Add<FriendUsageExample>()
      .Add<InheritanceUsageExample>()
      .Add<DynamicPolymorphismExample>()
      .Add<StaticPolymorphismExample>()
      .Add<DiamondProblemExample>()
      .Add<VirtualDestructorUsage>()
      .Add<GoogleNoDestructor>()
      // 3- smart pointers
      .Add<MallocUsageExample>()
      .Add<SmartPointerUsage>()
      .Add<SharedPointerUsage>()
      .Add<WeakPointerUsage>()
      .Add<OldStyleSmartPointer>()
      // 5- static usage
      .Add<StaticUsageExample>()
      // CPU Architecture
      .Add<CpuCacheExample>()
      .Add<CountIfRandom>()
      .Add<CountIfSorted>()
      .Add<VirtualCallsSequenced>()
      .Add<VirtualCallsShuffled>()
      .Add<AlignedStructAccess>()
      .Add<PackedStructAccess>()
      .Add<FalseSharingSingleAtomic>()
      .Add<FalseSharingAtomicsInSingleCacheLine>()
      .Add<FalseSharingResolved>()
      // 6- templates
      .Add<TemplateInheritanceUsageExample>()
      .Add<TemplateCrtpUsage>()
      .Add<TemplateContainerExample>()
      .Add<MixinTemplateExampleProblem>()
      .Add<MixinTemplateExample>()
      .Add<TemplatePolicies>()
      .Add<TemplateDecltypeUsage>()
      .Add<TemplateStaticAssert>()
      // 7- exceptions
      .Add<ExceptionUsage>()
      // 8- custom comparators
      .Add<CustomComparatorNeedExample>()
      .Add<CustomComparatorExample>()
      // 9- async example
      .Add<AsyncSequentialExample>()
      .Add<AsyncUsageExample>()
      .Add<AsyncFutureUsageExample>();
    po.PrintDetails();

    enum cmd_type {
      menu,
      quit,
      runall,
      cmd,
    };
   
    auto ParseCmdLine = [](const std::string& line) -> cmd_type {
      if (line == "q" || line == "quit") return{ cmd_type::quit };
      else if (line == "?" || line == "menu") return{ cmd_type::menu };
      else if (line == "r" || line == "all") return { cmd_type::runall };
      else return { cmd_type::cmd };
    };

    std::ostringstream o_str;
    o_str
      << "\n\n"
      << "Lutfen asagida belirtilenlerden birini girerek devam ediniz:\n"
      << "programdan cikmak icin: \"q\" \"quit\"\n"
      << "menu icin \"?\" veya \"menu\" yazarak\n"
      << "butun kodlari calistirmak icin: \"all\"\n"
      << "ilgili kodu calistirmak icin asagidaki yontemlerden birini uygulayin:\n"
      //<< " - \"id\" ile çalýþtýrmak için ilgili kodun id'si"
      << " - ismi ile calistirmak icin \"name: \" kisminda yazan ismi\n"
      << "girerek programa devam edebilirsiniz";
    std::cout << o_str.str() << std::endl;

    const std::string escape_char{"q"};
    std::string read_line{""};
    while (std::getline(std::cin, read_line))
    {
      //if (read_line == escape_char) break;
      switch (ParseCmdLine(read_line))
      {
      case cmd_type::menu:
        po.PrintDetails();
        break;
      case cmd_type::runall:
        po.RunAll();
        break;
      case cmd_type::cmd:
#ifdef WIN32
        // add mscv build decorations
        po.RunWithName({ std::string("class " + read_line) });
#else
        po.RunWithName({ read_line });
#endif
        break;
      case cmd_type::quit:
      default:
        return EXIT_SUCCESS;
      }
      std::cout << o_str.str() << std::endl;
    }

    std::cout << "Press enter to exit programm.." ;
    std::string s;
    std::getline(std::cin, s);
  return EXIT_SUCCESS;
}
