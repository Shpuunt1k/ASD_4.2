#include "pch.h"
#include "CppUnitTest.h"
#include "Source.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(encoding)
		{
			SFCode test;
			string toEncode = "test abcdefg";
			string coded = "1101110011101011101010011000010111011000";
			Assert::AreEqual(coded, test.encode(toEncode));
			toEncode = "";
			coded = "";
			Assert::AreEqual(coded, test.encode(toEncode));
		}
		TEST_METHOD(decoding)
		{
			SFCode encryptor;

			string toDecode = "1101110011101011101010011000010111011000";
			string goal = "test abcdefg";

			try
			{
				encryptor.decode(toDecode);
			}
			catch (const std::exception & ex)
			{
				Assert::AreEqual(ex.what(), "Can't decode before encode");
			}

			encryptor.encode(goal);

			toDecode = "110111001110101110101011010111010011000010111011000";

			try
			{
				encryptor.encode(toDecode);
			}
			catch (const std::exception & ex)
			{
				Assert::AreEqual(ex.what(), "Can't decode");
			}

			toDecode = encryptor.encode(goal);
			Assert::AreEqual(goal, encryptor.decode(toDecode));
		}
	};
}