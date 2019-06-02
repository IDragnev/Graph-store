#include "CppUnitTest.h"
#include "String\String.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::String;

namespace StringTest
{		
	TEST_CLASS(StringTest)
	{
	public:
		TEST_METHOD(DefaultConstructedStringIsEmpty)
		{
			String str{};

			Assert::IsTrue(isTheEmptyString(str));
		}

		TEST_METHOD(CStringConstructorWithNonEmptyCString)
		{
			String str{ C_STRING_PROTOTYPE };

			Assert::IsTrue(areEqual(str, C_STRING_PROTOTYPE));
		}

		TEST_METHOD(CStringConstructorWithEmptyCStringConstructsEmptyString)
		{
			String str{ "" };

			Assert::IsTrue(isTheEmptyString(str));
		}

		TEST_METHOD(CStringConstructorWithNullptrConstructsEmptyString)
		{
			String str{ nullptr };

			Assert::IsTrue(isTheEmptyString(str));
		}

		TEST_METHOD(SingleCharConstructor)
		{
			String str{ 'C' };

			Assert::IsTrue(areEqual(str, "C"));
		}

		TEST_METHOD(CopyCtorWithEmptyString)
		{
			String source;
			auto destination = source;

			Assert::IsTrue(destination == source);
		}

		TEST_METHOD(CopyCtorWithNonEmptyString)
		{
			String source{ C_STRING_PROTOTYPE };
			auto destination = source;

			Assert::IsTrue(destination == source);
		}

		TEST_METHOD(CopyAssignmentEmptyToEmpty)
		{
			String rhs;
			String lhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(CopyAssignmentNonEmptyToEmpty)
		{
			String lhs;
			String rhs{ C_STRING_PROTOTYPE };

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(CopyAssignmentEmptyToNonEmpty)
		{
			String lhs{ C_STRING_PROTOTYPE };
			String rhs;

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(CopyAssignmentNonEmptyToNonEmpty)
		{
			String lhs{ C_STRING_PROTOTYPE };
			String rhs{ "something different" };

			lhs = rhs;

			Assert::IsTrue(lhs == rhs);
		}

		TEST_METHOD(MoveCtorFromNonEmptySource)
		{
			String source{ C_STRING_PROTOTYPE };
			auto destination = std::move(source);

			Assert::IsTrue(areEqual(destination, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(source), L"Moved-from object is not empty");
		}

		TEST_METHOD(MoveCtorFromEmptySource)
		{
			String source;
			auto destination = std::move(source);

			Assert::IsTrue(isTheEmptyString(destination), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(source), L"Moved-from object is not empty");
		}

		TEST_METHOD(MoveAssignmentFromEmpty)
		{
			String lhs{ C_STRING_PROTOTYPE };
			String rhs;

			lhs = std::move(rhs);

			Assert::IsTrue(isTheEmptyString(lhs), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(rhs), L"Moved-from object is not empty");
		}

		TEST_METHOD(MoveAssignmentFromNonEmpty)
		{
			String lhs;
			String rhs{ C_STRING_PROTOTYPE };

			lhs = std::move(rhs);

			Assert::IsTrue(areEqual(lhs, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(rhs), L"Moved-from object is not empty");
		}

		TEST_METHOD(AppendingEmptyStringToEmptyDestination)
		{
			String empty;

			empty += "";

			Assert::IsTrue(isTheEmptyString(empty));
		}

		TEST_METHOD(AppendingNonEmptyToEmpty)
		{
			String str;

			str += C_STRING_PROTOTYPE;

			Assert::IsTrue(areEqual(str, C_STRING_PROTOTYPE));
		}

		TEST_METHOD(AppendingNonEmptyToNonEmptyString)
		{
			String str{ "012345" };

			str += "6789";

			Assert::IsTrue(areEqual(str, "0123456789"));
		}

		TEST_METHOD(AppendingEmptyToNonEmptyDestination)
		{
			String str{ C_STRING_PROTOTYPE };

			str += "";

			Assert::IsTrue(areEqual(str, C_STRING_PROTOTYPE));
		}

	private:
		static bool areEqual(const String& lhs, const char* rhs)
		{
			return std::strcmp(lhs.getContent(), rhs) == 0u;
		}

		static bool isTheEmptyString(const String& string)
		{
			return areEqual(string, "");
		}

	private:
		static const char C_STRING_PROTOTYPE[];
	};

	const char StringTest::C_STRING_PROTOTYPE[] { "PROTOTYPE" };
}