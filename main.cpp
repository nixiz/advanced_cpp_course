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
      // Inheritance
      .Add<DiamondProblemExample>()
      .Add<VirtualDestructorUsage>()
      .Add<FriendUsageExample>()
      // Smart pointers
      .Add<MallocUsageExample>()
      .Add<SmartPointerUsage>()
      .Add<SharedPointerUsage>()
      .Add<WeakPointerUsage>()
      .Add<SharedPointerPoolUsage>()
      .Add<WeakPointerUsageRealWorld>()
      .Add<OldStyleSmartPointer>()
      // Static Usage
      .Add<StaticUsageExample>()
      // Exceptions
      .Add<ExceptionUsage>()
      // CPU Architecture
      .Add<CpuCacheExample>()
      .Add<CountIfRandom>()
      .Add<CountIfSequenced>()
      .Add<CountIfSorted>()
      .Add<VirtualCallsSequenced>()
      .Add<VirtualCallsShuffled>()
      .Add<AlignedStructAccess>()
      .Add<PackedStructAccess>()
      .Add<FalseSharingSingleAtomic>()
      .Add<FalseSharingAtomicsInSingleCacheLine>()
      .Add<FalseSharingResolved>()
      // Templates
      .Add<TemplateInheritanceUsageExample>()
      .Add<GoogleNoDestructor>()
      .Add<TemplateCrtpUsage>()
      .Add<TemplateContainerExample>()
      .Add<MixinTemplateExampleProblem>()
      .Add<MixinTemplateExample>()
      .Add<TemplatePolicies>()
      // Custom Comparators
      .Add<CustomComparatorNeedExample>()
      .Add<CustomComparatorExample>()
      // Modern C++ Examples
      .Add<AsyncSequentialExample>()
      .Add<AsyncUsageExample>()
      .Add<AsyncFutureUsageExample>()
      .Add<ReturnTypeDecleration>()
      .Add<TemplateStaticAssert>();
    po.PrintDetails();

    enum cmd_type {
      menu,
      quit,
      runall,
      cmd,
    };
   
    auto ParseCmdLine = [](const std::string& line) -> cmd_type {
      if (line == "q" || line == "quit") return cmd_type::quit ;
      else if (line == "?" || line == "menu") return cmd_type::menu;
      else if (line == "r" || line == "all") return  cmd_type::runall;
      else return cmd_type::cmd;
    };

    std::ostringstream o_str;
    o_str
      << "\n"
      << "\nplease enter one of following commands:"
      << "\nquit program: \"q\" or \"quit\""
      << "\nfor available items \"?\" or \"menu\""
      << "\nrun all: \"all\""
      << "\nor enter name of item you want to run:";
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
        po.RunWithName({ read_line });
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
