#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../Graph store/String/String.h"

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
		TEST_METHOD(testDefaultConstructedStringIsEmpty)
		{
			auto str = String{};

			Assert::IsTrue(isTheEmptyString(str));
		}

		TEST_METHOD(testCStringConstructorWithNonEmptyCString)
		{
			auto str = String{ C_STRING_PROTOTYPE };

			Assert::IsTrue(haveSameContents(str, C_STRING_PROTOTYPE));
		}

		TEST_METHOD(testCStringConstructorWithEmptyCStringConstructsEmptyString)
		{
			auto str = String{ "" };

			Assert::IsTrue(isTheEmptyString(str));
		}

		TEST_METHOD(testCStringConstructorWithNullptrConstructsEmptyString)
		{
			auto str = String{ nullptr };

			Assert::IsTrue(isTheEmptyString(str));
		}

		TEST_METHOD(testSingleCharConstructor)
		{
			auto str = String{ 'C' };

			Assert::IsTrue(haveSameContents(str, "C"));
		}

		TEST_METHOD(testCopyCtorWithEmptyString)
		{
			auto emptySource = String{};
			auto destination = emptySource;
			
			Assert::IsTrue(haveSameContents(destination, emptySource));
		}

		TEST_METHOD(testCopyCtorWithNonEmptyString)
		{
			auto source = String{ C_STRING_PROTOTYPE };
			auto destination = source;

			Assert::IsTrue(haveSameContents(source, destination));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			auto rhs = String{};
			auto lhs = String{};

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			auto lhs = String{};
			auto rhs = String{ C_STRING_PROTOTYPE };

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			auto lhs = String{ C_STRING_PROTOTYPE };
			auto rhs = String{};

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			auto lhs = String{ C_STRING_PROTOTYPE };
			auto rhs = String{ "something different" };

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testMoveCtorFromNonEmptySourceLeavesSourceEmpty)
		{
			auto source = String{ C_STRING_PROTOTYPE };
			auto destination = String{ std::move(source) };

			Assert::IsTrue(haveSameContents(destination, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(source), L"Moved-from object is not empty");
		}

		TEST_METHOD(testMoveCtorFromEmptySourceDoesNotModifySource)
		{
			auto emptySource = String{};
			auto destination = String{ std::move(emptySource) };

			Assert::IsTrue(isTheEmptyString(destination), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(emptySource), L"Moved-from object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyRhsLeavesLhsAndRhsEmpty)
		{
			auto lhs = String{ C_STRING_PROTOTYPE };
			auto emptyRHS = String{};

			lhs = std::move(emptyRHS);

			Assert::IsTrue(isTheEmptyString(lhs), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(emptyRHS), L"Moved-from object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyRhsLeavesRhsEmpty)
		{
			auto lhs = String{};
			auto rhs = String{ C_STRING_PROTOTYPE };

			lhs = std::move(rhs);

			Assert::IsTrue(haveSameContents(lhs, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(rhs), L"Moved-from object is not empty");
		}

		TEST_METHOD(testAppendingEmptyStringToEmptyDestinationLeavesTheDestinationEmpty)
		{
			auto emptyString = String{};

			emptyString.append("");

			Assert::IsTrue(isTheEmptyString(emptyString));
		}

		TEST_METHOD(testAppendingStringToEmptyString)
		{
			auto str = String{};

			str.append(C_STRING_PROTOTYPE);

			Assert::IsTrue(haveSameContents(str, C_STRING_PROTOTYPE));
		}

		TEST_METHOD(testAppendingNonEmptyStringToNonEmptyString)
		{
			auto str = String{ "012345" };

			str.append("6789");
			
			Assert::IsTrue(haveSameContents(str, "0123456789"));
		}

		TEST_METHOD(testAppendingEmptyStringToNonEmptyDestinationDoesNotModifyDestination)
		{
			auto str = String{ C_STRING_PROTOTYPE };

			str.append("");

			Assert::IsTrue(haveSameContents(str, C_STRING_PROTOTYPE));
		}
	};

	const char StringTest::C_STRING_PROTOTYPE[] { "PROTOTYPE" };
}