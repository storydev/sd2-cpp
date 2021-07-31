#pragma once

#include "stb_ds.h"
#include <string>

namespace StoryDev
{

    static int GLOBAL_ID = 0;

    enum CommandType
    {
        CHARACTER,
        BLOCK_START,
        NARRATIVE,
        DIALOGUE,
        OVERLAY_TITLE,
        CODE_LINE,
        INTERNAL_DIALOGUE,
        NEW_CONVO,
        CHOICES,
        DIALOGUE_BLOCK,
        GOTO,
        FALLTHROUGH,
        OPTION,
        OPTION_CONDITIONAL
    };

    struct Command
    {
        int ID;
        int Type;
        std::string *Data;

        Command()
        {
            Data = NULL;
        }
    };

    struct CommandBlock
    {
        int ID;
        std::string Title;
        bool IsExclusive;
        bool ClearCurrent;
        std::string ResourceOrigin;
        Command *Commands;
        std::string *ExtraData;
        std::string *Options;

        CommandBlock()
        {
            Commands = NULL;
            ExtraData = NULL;
            Options = NULL;
            ClearCurrent = true;
            IsExclusive = false;
            ID = 0;
        }
    };

    Command CreateCharacterCommand(std::string charName, std::string color);
    Command CreateBlockTitle(std::string name);
    Command CreateNarrative(std::string description);
    Command CreateDialogue(std::string character, std::string dialogue);
    Command CreateDialogueBlock(std::string character, std::string *states);
    Command CreateOverlayTitle(std::string title);
    Command CreateCodeLine(std::string code);
    Command CreateInternalDialogue(std::string character, std::string text);
    Command CreateNewConvo(std::string title);
    Command CreateChoices(std::string *choices);
    Command CreateGoto(std::string convoName);
    Command CreateOptionConditional(std::string conditional);
    Command CreateOption(std::string text);
    Command CreateFallThrough(std::string code);

    struct Parser
    {
        CommandBlock *_blocks;
        Command *_commands;
        bool isAChoice;
        bool isDialogueBlock;
        std::string *choices;
        CommandBlock currentBlock;
        std::string *addedResources;
        int blocksAdded;

        Parser()
        {
            _blocks = NULL;
            _commands = NULL;
            addedResources = NULL;
            currentBlock = CommandBlock();
        }
    };

    void ParserClear(Parser *parser);
    int Validate(std::string content, std::string file);
    
    struct WordLine
    {
        std::string Word;
        std::string Line;

        WordLine(std::string word, std::string line)
        {
            Word = word;
            Line = line;
        }
    };

    static WordLine GetNextWord(std::string value)
    {
        std::string result;
        int index = 0;
        for (int i = 0; i < value.length(); i++)
        {
            char c = value[i];
            index++;
            if (c == ' ')
                break;
            else
                result += c;
        }

        value = value.substr(index);
        return WordLine(result, value);
    }

    static bool CheckChoices(Parser *parser)
    {
        bool result = false;
        if (parser->isAChoice && parser->currentBlock.Commands != NULL)
        {
            arrpush(parser->currentBlock.Commands, CreateChoices(parser->choices));
            parser->isAChoice = false;
            parser->choices = NULL;

            result = true; 
        }

        return result;
    }

    int ParseFile(Parser *parser, std::string file);


}