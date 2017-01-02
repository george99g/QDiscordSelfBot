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

#ifndef HELPCOMMAND_HPP
#define HELPCOMMAND_HPP

#include "command.hpp"

class HelpCommand : public Command
{
public:
	//Require a reference to the prefix so we know the command syntax.
	HelpCommand(QDiscord& _discord, QString& prefix);
	void dispatch(QDiscordMessage message, QStringList args);
	QString commandName();
	QString helpText();
	QStringList argumentText();
private:
	QString& _prefix;
};

#endif // HELPCOMMAND_HPP
