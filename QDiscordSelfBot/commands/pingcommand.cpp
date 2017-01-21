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

#include "pingcommand.hpp"

PingCommand::PingCommand(QDiscord& discord)
:Command(discord)
{

}

void PingCommand::dispatch(QDiscordMessage message, QStringList args)
{
	Q_UNUSED(args);
	//Ping pong!
	_discord.rest()->editMessage("Pong!", message);
}

QString PingCommand::commandName()
{
	return "ping";
}

QString PingCommand::helpText()
{
	return "Immediately responds with a pong.";
}

QStringList PingCommand::argumentText()
{
	return QStringList();
}
