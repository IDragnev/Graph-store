#include "CppUnitTest.h"
#include "../../../Graph store/String/String.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using IDragnev::String;

namespace StringTest
{		
	bool haveSameContents(const char* lhs, const char* rhs)
	{
		return strcmp(lhs, rhs) == 0;
	}

	bool isTheEmptyString(const String& string)
	{
		return haveSameContents(string, "");
	}

	TEST_CLASS(StringTest)
	{
	private:
		static const char C_STRING_PROTOTYPE[];

	public:
		TEST_METHOD(DefaultConstructedStringIsEmpty)
		{
			String str{};

			Assert::IsTrue(isTheEmptyString(str));
		}

		TEST_METHOD(CStringConstructorWithNonEmptyCString)
		{
			String str{ C_STRING_PROTOTYPE };

			Assert::IsTrue(haveSameContents(str, C_STRING_PROTOTYPE));
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

			Assert::IsTrue(haveSameContents(str, "C"));
		}

		TEST_METHOD(CopyCtorWithEmptyString)
		{
			auto emptySource = String{};
			auto destination = emptySource;
			
			Assert::IsTrue(haveSameContents(destination, emptySource));
		}

		TEST_METHOD(CopyCtorWithNonEmptyString)
		{
			auto source = String{ C_STRING_PROTOTYPE };
			auto destination = source;

			Assert::IsTrue(haveSameContents(source, destination));
		}

		TEST_METHOD(CopyAssignmentEmptyToEmpty)
		{
			auto rhs = String{};
			auto lhs = String{};

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(CopyAssignmentNonEmptyToEmpty)
		{
			auto lhs = String{};
			auto rhs = String{ C_STRING_PROTOTYPE };

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(CopyAssignmentEmptyToNonEmpty)
		{
			auto lhs = String{ C_STRING_PROTOTYPE };
			auto rhs = String{};

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(CopyAssignmentNonEmptyToNonEmpty)
		{
			auto lhs = String{ C_STRING_PROTOTYPE };
			auto rhs = String{ "something different" };

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(MoveCtorFromNonEmptySourceLeavesSourceEmpty)
		{
			String source{ C_STRING_PROTOTYPE };
			auto destination = String{ std::move(source) };

			Assert::IsTrue(haveSameContents(destination, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(source), L"Moved-from object is not empty");
		}

		TEST_METHOD(MoveCtorFromEmptySourceDoesNotModifySource)
		{
			String emptySource{};
			auto destination = String{ std::move(emptySource) };

			Assert::IsTrue(isTheEmptyString(destination), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(emptySource), L"Moved-from object is not empty");
		}

		TEST_METHOD(MoveAssignmentFromEmptyRhsLeavesLhsAndRhsEmpty)
		{
			auto lhs = String{ C_STRING_PROTOTYPE };
			auto emptyRHS = String{};

			lhs = std::move(emptyRHS);

			Assert::IsTrue(isTheEmptyString(lhs), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(emptyRHS), L"Moved-from object is not empty");
		}

		TEST_METHOD(MoveAssignmentFromNonEmptyRhsLeavesRhsEmpty)
		{
			auto lhs = String{};
			auto rhs = String{ C_STRING_PROTOTYPE };

			lhs = std::move(rhs);

			Assert::IsTrue(haveSameContents(lhs, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(rhs), L"Moved-from object is not empty");
		}

		TEST_METHOD(AppendingEmptyStringToEmptyDestinationLeavesTheDestinationEmpty)
		{
			auto emptyString = String{};

			emptyString.append("");

			Assert::IsTrue(isTheEmptyString(emptyString));
		}

		TEST_METHOD(AppendingStringToEmptyString)
		{
			auto str = String{};

			str.append(C_STRING_PROTOTYPE);

			Assert::IsTrue(haveSameContents(str, C_STRING_PROTOTYPE));
		}

		TEST_METHOD(AppendingNonEmptyStringToNonEmptyString)
		{
			auto str = String{ "012345" };

			str.append("6789");
			
			Assert::IsTrue(haveSameContents(str, "0123456789"));
		}

		TEST_METHOD(AppendingEmptyStringToNonEmptyDestinationDoesNotModifyDestination)
		{
			auto str = String{ C_STRING_PROTOTYPE };

			str.append("");

			Assert::IsTrue(haveSameContents(str, C_STRING_PROTOTYPE));
		}
	};

	const char StringTest::C_STRING_PROTOTYPE[] { "PROTOTYPE" };
}