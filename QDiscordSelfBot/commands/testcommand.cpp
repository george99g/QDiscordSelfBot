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

#include "testcommand.hpp"

TestCommand::TestCommand(QDiscord& discord)
:Command(discord)
{

}

void TestCommand::dispatch(QDiscordMessage message, QStringList args)
{
	//This command is useful for testing what arguments you're actually
	//passing in.
	QString text = "**Full message text:**\n";
	text += message.content() + "\n";
	if(args.length() > 0)
	{
		text += "**Arguments:**\n{`";
		text += args.join("`, `") + "`}";
	}
	_discord.rest()->editMessage(message, text);
}

QString TestCommand::commandName()
{
	return "test";
}

QString TestCommand::helpText()
{
	return "Test the interpreter's parsing.";
}

QStringList TestCommand::argumentText()
{
	return QStringList();
}
