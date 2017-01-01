/*
 * QDiscordSelfBot - An example self-bot written using the QDiscord library.
 * Copyright (C) 2017 george99g
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.	 If not, see <http://www.gnu.org/licenses/>.
 */

#include "helpcommand.hpp"
#include "commandmanager.hpp"

HelpCommand::HelpCommand(QDiscord& _discord, QString& prefix)
:Command(_discord), _prefix(prefix)
{

}

void HelpCommand::dispatch(QDiscordMessage message, QStringList args)
{
	if(args.length() > 1)
	{
		_discord.rest()->editMessage(message, "Too many arguments.");
		return;
	}
	//Give information for one command in case it's specified.
	if(args.length() == 1)
	{
		Command* command =CommandManager::instance().command(args[0]);
		if(!command)
		{
			_discord.rest()->editMessage(message, "Command not found.");
			return;
		}
		QString text = "**" + command->commandName() + ":**\n";
		text += "**Description:**\n";
		text += command->helpText() + "\n";
		text += "**Syntax:** `" + _prefix + command->commandName();
		if(command->argumentText().length() == 0)
			text += "`\n";
		else
		{
			text += " " + command->argumentText()[0] + "`\n";
			text += "**Where:**";
			for(const QString arg : command->argumentText().mid(1))
				text += "\n" + arg;
		}
		_discord.rest()->editMessage(message, text);
		return;
	}
	//If not, just list all of the commands.
	QString text = "All available commands:\n`";
	text += CommandManager::instance().commands().keys().join("`, `");
	text += "`.";
	_discord.rest()->editMessage(message, text);
}

QString HelpCommand::commandName()
{
	return "help";
}

QString HelpCommand::helpText()
{
	return "Provides command descriptions and usage information.";
}

QStringList HelpCommand::argumentText()
{
	return {"[COMMAND]",
		"COMMAND is the command you need help with."
		"When omitted will display all available commands."};
}
