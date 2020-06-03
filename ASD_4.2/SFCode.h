#pragma once
#include <string>

#include "Spisok.h"
#include "Map.h"

class SFCode
{
public:
	string encode(string input)
	{
		short preWeight = input.size() * 8;
		Map<char, int> composition;
		for (int i = 0; i < input.size(); ++i)
		{
			char temp = input.at(i);

			if (composition.find(temp) == NULL)
				composition.insert(temp, 1);
			else
				composition.change(temp, composition.find(temp) + 1);
		}
		cout << input << endl;
		composition.printList();
		codes.clear();

		List<char> regulars;
		composition.get_keys(regulars);
		List<int> frquencies;
		composition.get_values(frquencies);

		for (int i = 0; i < regulars.GetSize(); ++i)
		{
			Node temp;
			temp.regular = regulars[i];
			temp.frequency = frquencies[i];
			codes.push_back(temp);
		}

		if (codes.GetSize() <= 2)
		{
			for (unsigned int i = 0; i < codes.GetSize(); ++i)
				codes[i].encoded = i;

			string answer;
			while (input.size() > 0)
				for (unsigned int i = 0; i < codes.GetSize(); ++i)
					if (codes[i].regular[0] == input[0])
					{
						answer += codes[i].encoded;
						input.erase(0, 1);
						break;
					}
			return answer;
		}

		List<Node> ground;

		while (codes.GetSize() > 1)
		{
			Node* small1;
			Node* small2;
			if (codes[0].frequency < codes[1].frequency)
			{
				small1 = &codes[0];
				small2 = &codes[1];
			}
			else
			{
				small1 = &codes[1];
				small2 = &codes[0];
			}

			for (int i = 2; i < codes.GetSize(); ++i)
			{
				if (codes[i].frequency < small1->frequency)
				{
					small2 = small1;
					small1 = &codes[i];
				}
				else if (codes[i].frequency < small2->frequency)
					small2 = &codes[i];
			}
			Node temp;

			temp.regular = small2->regular;
			temp.regular.append(small1->regular);

			ground.push_back(*small2);
			temp.left = &ground[ground.GetSize() - 1];

			ground.push_back(*small1);
			temp.right = &ground[ground.GetSize() - 1];

			temp.frequency = temp.right->frequency + temp.left->frequency;

			if (small1 < small2)
			{
				for (int i = 0; i < codes.GetSize(); ++i)
					if (&codes[i] == small2)
						codes.removeAt(i);
				for (int i = 0; i < codes.GetSize(); ++i)
					if (&codes[i] == small1)
						codes.removeAt(i);
			}
			else
			{
				for (int i = 0; i < codes.GetSize(); ++i)
					if (&codes[i] == small1)
						codes.removeAt(i);
				for (int i = 0; i < codes.GetSize(); ++i)
					if (&codes[i] == small2)
						codes.removeAt(i);
			}

			codes.push_back(temp);
		}

		codemaker(codes[0], "");

		codes.clear();

		for (int i = 0; i < ground.GetSize(); i++)
			if (ground[i].regular.size() == 1)
				codes.push_back(ground[i]);

		string rValue;
		while (input.size() > 0)
			for (int i = 0; i < codes.GetSize(); ++i)
				if (codes[i].regular[0] == input[0])
				{
					rValue += codes[i].encoded;
					input.erase(0, 1);
					break;
				}

		double postweight = rValue.size();
		auto rate = (1 - postweight / preWeight) * 100;
		cout << "Regular text weight: " << preWeight << " bytes, coded weight: " << postweight << " bytes, " << rate << " %" << endl;

		cout << endl << "Coded:" << endl << rValue << endl;
		return rValue;
	}
	void print()
	{
		for (int i = 0; i < codes.GetSize(); i++)
			cout << codes[i].regular << " - " << codes[i].encoded << endl;
	}
	string decode(string input)
	{
		double postweight = input.size();
		if (codes.GetSize() == 0)
			throw exception("Can't decode before encode");
		string current;
		string answer;
		while (input.size() > 0)
		{
			current += input[0];
			input.erase(0, 1);
			for (int i = 0; i < codes.GetSize(); ++i)
				if (codes[i].encoded == current)
				{
					answer += codes[i].regular;
					current.clear();
					if (input.size() == 0)
					{
						short preWeight = answer.size() * 8;

						auto rate = (1 - postweight / preWeight) * 100;
						cout << "Regular weight: " << preWeight << " bytes, coded weight: " << postweight << " bytes, " << rate << " %" << endl;

						cout << endl << "Decoded:" << endl << answer << endl;
						return answer;
					}
				}
		}
		throw exception("Can't decode");
	}

private:
	struct Node
	{
		string regular;
		string encoded;
		int frequency;
		Node* left;
		Node* right;
	};

	void codemaker(Node& node, string code)
	{
		if (node.regular.size() == 1)
		{
			node.encoded = code;
			return;
		}
		string temp0 = code;
		temp0.append("0");
		codemaker(*node.right, temp0);
		string temp1 = code;
		temp1.append("1");
		codemaker(*node.left, temp1);
	}

	List<Node> codes;
};