#include "Pch.h"
#include "CppUnitTest.h"
#include "Scope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace std;

namespace LibraryDesktopTest
{
	TEST_CLASS(ScopeTest)
	{
	public:
		TEST_METHOD(ScopeTestDefaultConstructor)
		{
			Scope scopeOne;
			Scope scopeTwo;

			Assert::IsTrue(scopeOne == scopeTwo);
			Assert::IsTrue(scopeOne.GetParent() == nullptr);
		}

		TEST_METHOD(ScopeTestFind)
		{
			Scope scope;
			Datum* datumPointer;
			Datum datum;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			Datum savedNameOneDatum;
			int32_t tempInt = 10;

			datumPointer = scope.Find(nameOne);
			Assert::IsTrue(datumPointer == nullptr);

			//Appending first datum
			datum = scope.Append(nameOne);
			Assert::IsTrue(datum == Datum());

			scope.Append(nameOne).SetType(DatumType::INT32_T);
			scope.Append(nameOne).PushBack(tempInt);
			savedNameOneDatum = scope.Append(nameOne);
			Assert::IsTrue(savedNameOneDatum.Type() == DatumType::INT32_T);

			//Appending second datum
			datum = scope.Append(nameTwo);
			Assert::IsTrue(datum == Datum());
			Assert::IsTrue(savedNameOneDatum == *scope.Find(nameOne));

			Assert::IsTrue(scope.Find(nameThree) == nullptr);

			//Appending third datum
			scope.Append(nameThree);
			Assert::IsTrue(scope.Find(nameThree, nullptr) == nullptr);
			scope.Append(nameThree).SetType(DatumType::INT32_T);
			scope.Append(nameThree).PushBack(tempInt);

			datum = *scope.Find(nameThree, &scope);
			Assert::IsTrue(datum.Type() == DatumType::INT32_T);
			Assert::IsTrue(datum.Get<int32_t>() == tempInt);

			Assert::IsTrue(*(scope.Find(nameOne, &scope)) == savedNameOneDatum);
		}

		TEST_METHOD(ScopeTestSearch)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			Assert::IsTrue(scope.Search(nameOne) == nullptr);
			Assert::IsTrue(scope.Search(nameOne, nullptr) == nullptr);

			scope.Append(nameOne).SetType(DatumType::INT32_T);
			scope.Append(nameTwo).SetType(DatumType::FLOAT);
			scope.Append(nameThree).SetType(DatumType::STRING);

			Assert::IsTrue(scope.Search(nameOne)->Type() == DatumType::INT32_T);
			Assert::IsTrue(scope.Search(nameTwo)->Type() == DatumType::FLOAT);
			Assert::IsTrue(scope.Search(nameThree)->Type() == DatumType::STRING);

			Scope *scopePointer = &scope;
			Assert::IsTrue(scope.Search(nameOne, &scopePointer)->Type() == DatumType::INT32_T);
			Assert::IsTrue(scope.Search(nameTwo, &scopePointer)->Type() == DatumType::FLOAT);
			Assert::IsTrue(scope.Search(nameThree, &scopePointer)->Type() == DatumType::STRING);

			//Appending nested scopes.
			string childOneScope = "childOneScope";
			string childTwoScope = "childTwoScope";

			string nameOneChildOne = "nameOneChildOne";
			string nameTwoChildOne = "nameTwoChildOne";
			string nameThreeChildOne = "nameThreeChildOne";

			Scope& childScope = scope.AppendScope(childOneScope);
			childScope.Append(nameOneChildOne).SetType(DatumType::GLM_VECTOR4);
			childScope.Append(nameTwoChildOne).SetType(DatumType::GLM_MATRIX4X4);
			childScope.Append(nameThreeChildOne).SetType(DatumType::TABLE);

			//Checking in child scope
			Assert::IsTrue(childScope.Search(nameOneChildOne)->Type() == DatumType::GLM_VECTOR4);
			Assert::IsTrue(childScope.Search(nameTwoChildOne)->Type() == DatumType::GLM_MATRIX4X4);
			Assert::IsTrue(childScope.Search(nameThreeChildOne)->Type() == DatumType::TABLE);

			scopePointer = &childScope;
			Assert::IsTrue(scope.Search(nameOneChildOne, &scopePointer)->Type() == DatumType::GLM_VECTOR4);
			Assert::IsTrue(scope.Search(nameTwoChildOne, &scopePointer)->Type() == DatumType::GLM_MATRIX4X4);
			Assert::IsTrue(scope.Search(nameThreeChildOne, &scopePointer)->Type() == DatumType::TABLE);

			scopePointer = &childScope;
			Assert::IsTrue(scope.Search(nameOneChildOne, &scopePointer)->Type() == DatumType::GLM_VECTOR4);
			Assert::IsTrue(scope.Search(nameTwoChildOne, &scopePointer)->Type() == DatumType::GLM_MATRIX4X4);
			Assert::IsTrue(scope.Search(nameThreeChildOne, &scopePointer)->Type() == DatumType::TABLE);

			//Rechecking again
			Assert::IsTrue(scope.Search(nameOne)->Type() == DatumType::INT32_T);
			Assert::IsTrue(scope.Search(nameTwo)->Type() == DatumType::FLOAT);
			Assert::IsTrue(scope.Search(nameThree)->Type() == DatumType::STRING);

			//Searching in parent scope
			Assert::IsTrue(childScope.Search(nameOne)->Type() == DatumType::INT32_T);
			Assert::IsTrue(childScope.Search(nameTwo)->Type() == DatumType::FLOAT);
			Assert::IsTrue(childScope.Search(nameThree)->Type() == DatumType::STRING);

			//Appending second level nested scope
			string nameOneChildTwo = "nameOneChildTwo";
			string nameTwoChildTwo = "nameTwoChildTwo";
			string nameThreeChildTwo = "nameThreeChildTwo";

			Scope& nestedChildScope = childScope.AppendScope(childTwoScope);
			nestedChildScope.Append(nameOneChildTwo).SetType(DatumType::INT32_T);
			nestedChildScope.Append(nameTwoChildTwo).SetType(DatumType::FLOAT);
			nestedChildScope.Append(nameThreeChildTwo).SetType(DatumType::STRING);

			//Checking in nested child scope
			Assert::IsTrue(nestedChildScope.Search(nameOneChildTwo)->Type() == DatumType::INT32_T);
			Assert::IsTrue(nestedChildScope.Search(nameTwoChildTwo)->Type() == DatumType::FLOAT);
			Assert::IsTrue(nestedChildScope.Search(nameThreeChildTwo)->Type() == DatumType::STRING);

			Assert::IsTrue(nestedChildScope.Search(nameOneChildOne)->Type() == DatumType::GLM_VECTOR4);
			Assert::IsTrue(nestedChildScope.Search(nameTwoChildOne)->Type() == DatumType::GLM_MATRIX4X4);
			Assert::IsTrue(nestedChildScope.Search(nameThreeChildOne)->Type() == DatumType::TABLE);

			Assert::IsTrue(nestedChildScope.Search(nameOne)->Type() == DatumType::INT32_T);
			Assert::IsTrue(nestedChildScope.Search(nameTwo)->Type() == DatumType::FLOAT);
			Assert::IsTrue(nestedChildScope.Search(nameThree)->Type() == DatumType::STRING);

			//Checking using pointers
			scopePointer = &nestedChildScope;
			Assert::IsTrue(nestedChildScope.Search(nameOneChildTwo, &scopePointer)->Type() == DatumType::INT32_T);
			Assert::IsTrue(nestedChildScope.Search(nameTwoChildTwo, &scopePointer)->Type() == DatumType::FLOAT);
			Assert::IsTrue(nestedChildScope.Search(nameThreeChildTwo, &scopePointer)->Type() == DatumType::STRING);

			scopePointer = &childScope;
			Assert::IsTrue(childScope.Search(nameOneChildOne, &scopePointer)->Type() == DatumType::GLM_VECTOR4);
			Assert::IsTrue(childScope.Search(nameTwoChildOne, &scopePointer)->Type() == DatumType::GLM_MATRIX4X4);
			Assert::IsTrue(childScope.Search(nameThreeChildOne, &scopePointer)->Type() == DatumType::TABLE);

			scopePointer = &scope;
			Assert::IsTrue(scope.Search(nameOne, &scopePointer)->Type() == DatumType::INT32_T);
			Assert::IsTrue(scope.Search(nameTwo, &scopePointer)->Type() == DatumType::FLOAT);
			Assert::IsTrue(scope.Search(nameThree, &scopePointer)->Type() == DatumType::STRING);
		}

		TEST_METHOD(ScopeTestAppend)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = scope.Append(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope.Append(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = scope.Append(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			Datum testOne = scope.Append(nameOne);
			Assert::IsTrue(firstAppend == testOne);

			Datum testTwo = scope.Append(nameTwo);
			Assert::IsTrue(secondAppend == testTwo);

			Datum testThree = scope.Append(nameThree);
			Assert::IsTrue(thirdAppend == testThree);
		}

		TEST_METHOD(ScopeTestAppendScope)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			string nameOneChildOne = "nameOneChildOne";

			Scope& firstAppendedScope = scope.AppendScope(nameOne);
			Scope firstAppendedScopeCopy = firstAppendedScope;
			Assert::IsTrue(firstAppendedScope == firstAppendedScopeCopy);

			Datum& tempDatum = firstAppendedScope.Append(nameOneChildOne);
			tempDatum.SetType(DatumType::INT32_T);
			int32_t tempInt = 10;
			tempDatum.PushBack(tempInt);

			firstAppendedScopeCopy = firstAppendedScope;
			Assert::IsTrue(firstAppendedScope == firstAppendedScopeCopy);

			Datum& intDatum = scope.Append(nameTwo);
			intDatum.SetType(DatumType::INT32_T);
			intDatum.PushBack(tempInt);

			Assert::ExpectException<exception>([&] {scope.AppendScope(nameTwo); });

			Scope& secondAppendedScope = scope.AppendScope(nameThree);
			Scope emptyScope = scope.AppendScope(nameThree);

			secondAppendedScope.Clear();
		}

		TEST_METHOD(ScopeTestAdopt)
		{
			Scope scope;
			Scope parentOfChildScope;

			string childScopeString = "childScopeString";
			string tempString = "tempString";

			Assert::ExpectException<exception>([&] {scope.Adopt(parentOfChildScope, childScopeString); });

			Scope& childScope = parentOfChildScope.AppendScope(childScopeString);
			Assert::IsFalse(parentOfChildScope.Find(childScopeString) == nullptr);
			Assert::IsTrue(childScope.GetParent() == &parentOfChildScope);

			Assert::ExpectException<exception>([&] {scope.Adopt(childScope, tempString); });

			string childScopeData = "childScopeData";
			Datum& childScopeDatum = childScope.Append(childScopeData);
			childScopeDatum.SetType(DatumType::STRING);
			childScopeDatum.PushBack(tempString);

			scope.Adopt(childScope, childScopeString);
			Datum datum = *scope.Find(childScopeString);

			Assert::IsTrue(datum.Type() == DatumType::TABLE);
			Assert::IsTrue(*(datum.Get<Scope*>()) == childScope);

			Assert::IsTrue(parentOfChildScope.Find(childScopeString) == nullptr);
			Assert::IsFalse(childScope.GetParent() == &parentOfChildScope);

			Assert::IsTrue(childScope.GetParent() == &scope);
		}

		TEST_METHOD(ScopeTestGetParent)
		{
			Scope rootScope;
			Assert::IsTrue(rootScope.GetParent() == nullptr);

			string firstLevelChildString = "firstLevelChildString";
			Scope& firstLevelChildScope = rootScope.AppendScope(firstLevelChildString);
			Assert::IsTrue(&rootScope == firstLevelChildScope.GetParent());

			string firstLevelChildSiblingString = "firstLevelChildSiblingString";
			Scope& firstLevelChildSiblingScope = rootScope.AppendScope(firstLevelChildSiblingString);
			Assert::IsTrue(&rootScope == firstLevelChildSiblingScope.GetParent());

			string secondLevelChildString = "secondLevelChildString";
			Scope& secondLevelChildScope = firstLevelChildSiblingScope.AppendScope(secondLevelChildString);
			Assert::IsTrue(&firstLevelChildSiblingScope == secondLevelChildScope.GetParent());

			string thirdLevelChildString = "thirdLevelChildString";
			Scope& thirdLevelChildScope = secondLevelChildScope.AppendScope(thirdLevelChildString);
			Assert::IsTrue(&secondLevelChildScope == thirdLevelChildScope.GetParent());
		}

		TEST_METHOD(ScopeTestOverloadedSubscriptOperatorWithName)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			//Non const version of overloaded subscript operator
			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = scope[nameThree];
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			Datum testOne = scope[nameOne];
			Assert::IsTrue(firstAppend == testOne);

			Datum testTwo = scope[nameTwo];
			Assert::IsTrue(secondAppend == testTwo);

			Datum testThree = scope[nameThree];
			Assert::IsTrue(thirdAppend == testThree);

			//Const version of overloaded subscript operator			
			string nameFour = "nameFour";
			const Scope constScopeOne;
			Assert::ExpectException<exception>([&] {constScopeOne[nameFour]; });

			const Scope constScopeTwo = scope;
			Datum testOneConst = constScopeTwo[nameOne];
			Assert::IsTrue(firstAppend == testOneConst);

			Datum testTwoConst = constScopeTwo[nameTwo];
			Assert::IsTrue(secondAppend == testTwoConst);

			Datum testThreeConst = constScopeTwo[nameThree];
			Assert::IsTrue(thirdAppend == testThreeConst);
		}

		TEST_METHOD(ScopeTestOverloadedSubscriptOperatorWithIndex)
		{
			Scope scope;
			Scope *firstPushedScope = new Scope();
			uint32_t currentSize = 0;
			Assert::ExpectException<exception>([&] {scope[currentSize]; });

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;

			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(nameOne);

			Datum& thirdAppend = scope[nameThree];
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			Assert::IsTrue(firstAppend == scope[0]);
			Assert::IsTrue(secondAppend == scope[1]);
			Assert::IsTrue(thirdAppend == scope[2]);

			Assert::ExpectException<exception>([&] {scope[3]; });
			Assert::ExpectException<exception>([&] {scope[5]; });
		}

		TEST_METHOD(ScopeTestOverloadedEqualityOperator)
		{
			Scope scope;
			Assert::IsTrue(scope == scope);

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "FirstLevelNestedScope";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = scope[nameThree];
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			Scope testScopeOne;
			Assert::IsFalse(scope == testScopeOne);
			testScopeOne = scope;
			Assert::IsTrue(scope == testScopeOne);

			string secondLevelNestedScopeString = "secondLevelNestedScopeString";
			Scope& secondLevelNestedScope = firstPushedScope->AppendScope(nameThree);

			Datum& secondLevelNestedDatum = secondLevelNestedScope.Append(nameOne);
			secondLevelNestedDatum.SetType(DatumType::STRING);
			secondLevelNestedDatum.PushBack(firstPushedString);

			string thirdLevelNestedScopeString = "thirdLevelNestedScopeString";
			Scope& thirdLevelNestedScope = secondLevelNestedScope.AppendScope(thirdLevelNestedScopeString);

			Datum& thirdLevelNestedDatum = thirdLevelNestedScope.Append(nameOne);
			thirdLevelNestedDatum.SetType(DatumType::STRING);
			thirdLevelNestedDatum.PushBack(firstPushedString);

			Scope testScopeTwo = scope;
			Assert::IsTrue(scope == testScopeTwo);

			//Testing overridden .Equals()
			Datum testDatum = *scope.Find(nameOne);
			Assert::IsTrue(testDatum == firstAppend);

		}

		TEST_METHOD(ScopeTestOverloadedInEqualityOperator)
		{
			Scope scope;
			Assert::IsFalse(scope != scope);

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "FirstLevelNestedScope";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = scope[nameThree];
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			Scope testScopeOne;
			Assert::IsTrue(scope != testScopeOne);
			testScopeOne = scope;
			Assert::IsFalse(scope != testScopeOne);

			string secondLevelNestedScopeString = "secondLevelNestedScopeString";
			Scope& secondLevelNestedScope = firstPushedScope->AppendScope(nameThree);

			Datum& secondLevelNestedDatum = secondLevelNestedScope.Append(nameOne);
			secondLevelNestedDatum.SetType(DatumType::STRING);
			secondLevelNestedDatum.PushBack(firstPushedString);

			string thirdLevelNestedScopeString = "thirdLevelNestedScopeString";
			Scope& thirdLevelNestedScope = secondLevelNestedScope.AppendScope(thirdLevelNestedScopeString);

			Datum& thirdLevelNestedDatum = thirdLevelNestedScope.Append(nameOne);
			thirdLevelNestedDatum.SetType(DatumType::STRING);
			thirdLevelNestedDatum.PushBack(firstPushedString);

			Scope testScopeTwo = scope;
			Assert::IsFalse(scope != testScopeTwo);

			//Testing overridden .Equals()
			Datum testDatum = *scope.Find(nameOne);
			Assert::IsFalse(testDatum != firstAppend);
		}

		TEST_METHOD(ScopeTestFindName)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Assert::IsTrue(scope.FindName(firstPushedScope) == string());

			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Assert::IsTrue(scope.FindName(firstPushedScope) == string());

			Datum& thirdAppend = scope[nameThree];
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			Assert::IsTrue(scope.FindName(firstPushedScope) == nameThree);
			Scope *testScope = new Scope();
			Assert::IsTrue(scope.FindName(testScope) == string());

			thirdAppend.PushBack(testScope);
			Assert::IsTrue(scope.FindName(testScope) == nameThree);
		}

		TEST_METHOD(ScopeTestToString)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			Assert::IsTrue(scope.ToString() == string());

			//Checking ToString() for int, string and Table/Scope
			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = scope[nameThree];
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			string firstConvertedString = "INT32_T: 1, STRING: 1, TABLE: 1";
			Assert::IsTrue(scope.ToString() == firstConvertedString);

			//Checking ToString() for float, vec4, mat4x4
			float_t firstPushedFloat = 10.0;

			glm::vec4 firstPushedVec4(10, 20, 30, 40);
			glm::vec4 secondPushedVec4(50, 60, 70, 80);
			glm::vec4 thirdPushedVec4(90, 100, 110, 120);
			glm::vec4 fourthVec4(90, 100, 110, 120);

			glm::mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);

			string nameFour = "nameFour";
			string nameFive = "nameFive";
			string nameSix = "nameSix";

			Datum& fourthAppend = scope[nameFour];
			fourthAppend.SetType(DatumType::FLOAT);
			fourthAppend.PushBack(firstPushedFloat);

			Datum& fifthAppend = scope[nameFive];
			fifthAppend.SetType(DatumType::GLM_VECTOR4);
			fifthAppend.PushBack(firstPushedVec4);

			Datum& sixthAppend = scope[nameSix];
			sixthAppend.SetType(DatumType::GLM_MATRIX4X4);
			sixthAppend.PushBack(firstPushedMat4x4);

			string secondConvertedString = "INT32_T: 1, STRING: 1, TABLE: 1, FLOAT: 1, GLM_VECTOR4: 1, GLM_MATRIX4X4: 1";
			Assert::IsTrue(scope.ToString() == secondConvertedString);
		}

		TEST_METHOD(ScopeTestAssignmentOperator)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";

			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Scope& childScopeLevelOne = scope.AppendScope(nameThree);

			//Inserting into first level nested scope
			Datum& firstAppendLevelOne = childScopeLevelOne[nameOne];
			firstAppendLevelOne.SetType(DatumType::INT32_T);
			firstAppendLevelOne.PushBack(firstPushedInt);

			Datum& secondAppendLevelOne = childScopeLevelOne[nameTwo];
			secondAppendLevelOne.SetType(DatumType::STRING);
			secondAppendLevelOne.PushBack(firstPushedString);

			Scope& childScopeLevelTwo = childScopeLevelOne.AppendScope(nameThree);

			//Inserting into second level nested scope
			Datum& firstAppendLevelTwo = childScopeLevelTwo[nameOne];
			firstAppendLevelTwo.SetType(DatumType::INT32_T);
			firstAppendLevelTwo.PushBack(firstPushedInt);

			Datum& secondAppendLevelTwo = childScopeLevelTwo[nameTwo];
			secondAppendLevelTwo.SetType(DatumType::STRING);
			secondAppendLevelTwo.PushBack(firstPushedString);

			Datum& thirdAppendLevelTwo = childScopeLevelTwo.Append(nameThree);
			thirdAppendLevelTwo.SetType(DatumType::TABLE);

			Scope testScopeOne;
			testScopeOne = scope;
			Assert::IsTrue(testScopeOne == scope);
			Assert::IsTrue(testScopeOne.GetParent() == nullptr);
			secondAppendLevelTwo.Set("testFalse");
			Assert::IsFalse(testScopeOne == scope);

			Scope testScopeTwo;
			testScopeTwo = childScopeLevelOne;
			Assert::IsTrue(testScopeTwo == childScopeLevelOne);
			Assert::IsTrue(testScopeTwo.GetParent() == nullptr);
			secondAppendLevelOne.Set("testFalse");
			Assert::IsFalse(testScopeTwo == childScopeLevelOne);

			Scope testScopeThree;
			testScopeThree = childScopeLevelTwo;
			Assert::IsTrue(testScopeThree == childScopeLevelTwo);
			Assert::IsTrue(testScopeThree.GetParent() == nullptr);
			secondAppendLevelTwo.Set("something");
			Assert::IsFalse(testScopeThree == childScopeLevelTwo);
		}

		TEST_METHOD(ScopeTestCopyConstructor)
		{
			Scope scope;
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";

			Datum& firstAppend = scope[nameOne];
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = scope[nameTwo];
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Scope& childScopeLevelOne = scope.AppendScope(nameThree);

			//Inserting into first level nested scope
			Datum& firstAppendLevelOne = childScopeLevelOne[nameOne];
			firstAppendLevelOne.SetType(DatumType::INT32_T);
			firstAppendLevelOne.PushBack(firstPushedInt);

			Datum& secondAppendLevelOne = childScopeLevelOne[nameTwo];
			secondAppendLevelOne.SetType(DatumType::STRING);
			secondAppendLevelOne.PushBack(firstPushedString);

			Scope& childScopeLevelTwo = childScopeLevelOne.AppendScope(nameThree);

			//Inserting into second level nested scope
			Datum& firstAppendLevelTwo = childScopeLevelTwo[nameOne];
			firstAppendLevelTwo.SetType(DatumType::INT32_T);
			firstAppendLevelTwo.PushBack(firstPushedInt);

			Datum& secondAppendLevelTwo = childScopeLevelTwo[nameTwo];
			secondAppendLevelTwo.SetType(DatumType::STRING);
			secondAppendLevelTwo.PushBack(firstPushedString);

			childScopeLevelTwo.AppendScope(nameThree);

			Scope testScopeOne = scope;
			Assert::IsTrue(testScopeOne == scope);
			Assert::IsTrue(testScopeOne.GetParent() == nullptr);
			secondAppendLevelTwo.Set("testFalse");
			Assert::IsFalse(testScopeOne == scope);

			Scope testScopeTwo = childScopeLevelOne;
			Assert::IsTrue(testScopeTwo == childScopeLevelOne);
			Assert::IsTrue(testScopeTwo.GetParent() == nullptr);
			secondAppendLevelOne.Set("testFalse");
			Assert::IsFalse(testScopeTwo == childScopeLevelOne);

			Scope testScopeThree = childScopeLevelTwo;
			Assert::IsTrue(testScopeThree == childScopeLevelTwo);
			Assert::IsTrue(testScopeThree.GetParent() == nullptr);
			secondAppendLevelTwo.Set("something");
			Assert::IsFalse(testScopeThree == childScopeLevelTwo);
		}
	};
}