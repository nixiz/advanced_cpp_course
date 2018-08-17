#include <playground_organizer.hpp>
#include <iostream>
#include <string>

// 1- operator overloading, overriding
#include "src/1-OperatorOverload/op_overload_override_example.hpp"

// 2- inheritance
#include "src/2-Inheritance/friend_usage.hpp"
#include "src/2-Inheritance/inheritance_usage.hpp"
#include "src/2-Inheritance/template_inheritance_usage.hpp"
#include "src/2-Inheritance/diamond_problem.hpp"
#include "src/2-Inheritance/virtual_destructor.hpp"

// 3- smart pointers
#include "src/3-SmartPointer/malloc_usage.hpp"
#include "src/3-SmartPointer/smart_pointer.hpp"
#include "src/3-SmartPointer/shared_pointer.hpp"
#include "src/3-SmartPointer/weak_pointer.hpp"
#include "src/3-SmartPointer/bonus/bonus_old_style_smart_pointer.hpp"

// 4- reference usage
#include "src/4-RefUsage/ref_usage_example.hpp"
#include "src/4-RefUsage/copy_elision_usage.hpp"

// 5- static usage
#include "src/5-Static/static_usage.hpp"

// 6- templates
#include "src/6-Templates/template_mixin.hpp"
#include "src/6-Templates/template_staticpolymorphism.hpp"
#include "src/6-Templates/template_crtp_usage.hpp"
#include "src/6-Templates/template_decltype.hpp"
#include "src/6-Templates/template_container.hpp"
#include "src/6-Templates/template_policies.hpp"

// 7- exceptions
#include "src/7-Exceptions/exception_usage.hpp"

// 8- custom comparators
#include "src/8-CustomComparators/custom_comparator.hpp"

// 9- async
#include "src/9-AsyncFuture/async_usage.hpp"


int CppOrganizer::ICodeRunnerIdentifier::quest_id = 0;
int main(int argc, char *argv[]) {
    CppOrganizer::PlaygroundOrganizer po;
    po.builder()
      // 1- operator overloading, overriding
      .Add<OperatorOverloadOverrideExample>()
      // 2- inheritance
      .Add<FriendUsageExample>()
      .Add<InheritanceUsageExample>()
      .Add<DynamicPolymorphismExample>()
      .Add<StaticPolymorphismExample>()
      .Add<TemplateInheritanceUsageExample>()
      .Add<DiamondProblemExample>()
      .Add<VirtualDestructorUsage>()
      .Add<GoogleNoDestructor>()
      // 3- smart pointers
      .Add<MallocUsageExample>()
      .Add<SmartPointerUsage>()
      .Add<SharedPointerUsage>()
      .Add<WeakPointerUsage>()
      .Add<OldStyleSmartPointer>()
      // 4- reference usage
      .Add<RefUsageExample>()
      .Add<CopyElisionReturnByValue>()
      .Add<CopyElisionReturnByRefRef>()
      // 5- static usage
      .Add<StaticUsageExample>()
      // 6- templates
      .Add<TemplateCrtpUsage>()
      .Add<TemplateContainerExample>()
      .Add<MixinTemplateExampleProblem>()
      .Add<MixinTemplateExample>()
      .Add<TemplatePolicies>()
      .Add<TemplateDecltypeUsage>()
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
