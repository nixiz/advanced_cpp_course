#include <playground_organizer.hpp>
#include <iostream>
#include <string>
#include <sstream>

#include "advanced_cpp_topics.h"


int CppOrganizer::ICodeRunnerIdentifier::quest_id = 0;
int main(int argc, char *argv[]) {
  CppOrganizer::PlaygroundOrganizer po;
  po.builder()
    // Inheritance
    .Add<InheritanceUsageExample>()
    .Add<InheritanceAccessControl>()
    .Add<InheritanceVirtualFunctions>()
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
    if (line == "q" || line == "quit") return cmd_type::quit;
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

  const std::string escape_char{ "q" };
  std::string read_line{ "" };
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

  std::cout << "Press enter to exit programm..";
  std::string s;
  std::getline(std::cin, s);
  return EXIT_SUCCESS;
}
