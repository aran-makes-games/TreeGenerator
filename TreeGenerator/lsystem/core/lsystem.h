#ifndef TREE_GENERATOR_LSYSTEM_H_
#define TREE_GENERATOR_LSYSTEM_H_

#include <map>
#include <string>
#include <vector>

namespace tree_generator
{
	namespace lsystem
	{
		enum class Symbol : char {};
		using RuleMap = std::map<Symbol, std::vector<Symbol>>;

		struct LSystem
		{
			std::vector<Symbol> axiom;
			RuleMap rules;
		};

		std::vector<Symbol> Generate(const LSystem& lSystem, int iterations);
		std::vector<Symbol> Iterate(
			const std::vector<Symbol>& previous, const RuleMap& rules);

		std::string ToString(Symbol symbol);
	}
}

#endif  // !TREE_GENERATOR_LSYSTEM_H_