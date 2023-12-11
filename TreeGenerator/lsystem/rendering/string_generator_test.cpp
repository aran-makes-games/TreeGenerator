#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "string_generator.h"

#include "../core/lsystem.h"

namespace tree_generator
{
	namespace lsystem
	{
		namespace
		{
			TEST(LSystemStringGeneratorTest, NoDefinitionsGeneratesEmptyString)
			{
				Symbol a{ 'a' };
				StringGenerator generator;
				EXPECT_THAT(generator.Generate({ a }), testing::IsEmpty());
			}

			TEST(LSystemStringGeneratorTest, NoSymbolsGeneratesEmptyString)
			{
				StringGenerator generator;
				EXPECT_THAT(generator.Generate({}), testing::IsEmpty());
			}

			TEST(LSystemStringGeneratorTest, SymbolGeneratesDefinitionString)
			{
				Symbol a{ 'a' };

				StringGenerator generator;
				generator.Define(a, "A");
				EXPECT_EQ(generator.Generate({ a }), "A");
			}

			TEST(LSystemStringGeneratorTest, RepeatedSymbolGeneratesRepeatedString)
			{
				Symbol a{ 'a' };

				StringGenerator generator;
				generator.Define(a, "A");
				EXPECT_EQ(generator.Generate({ a, a, a }), "AAA");
			}

			TEST(LSystemGeneratorTest, DifferentSymbolDefinitionsGenerateDifferentStrings)
			{
				Symbol a{ 'a' };
				Symbol b{ 'b' };

				StringGenerator generator;
				generator.Define(a, "A");
				generator.Define(b, "B");
				EXPECT_EQ(generator.Generate({ b, a, b }), "BAB");
			}

			TEST(LSystemGeneratorTest, SymbolsWithoutDefinitionAreIgnored)
			{
				Symbol a{ 'a' };
				Symbol b{ 'b' };

				StringGenerator generator;
				generator.Define(b, "B");
				EXPECT_EQ(generator.Generate({ b, a, b }), "BB");
			}

			TEST(LSystemGeneratorTest, SymbolRedefinitionUsesMostRecent)
			{
				Symbol a{ 'a' };

				StringGenerator generator;
				generator.Define(a, "A");
				generator.Define(a, "B");

				EXPECT_EQ(generator.Generate({ a }), "B");
			}
		}
	}
}