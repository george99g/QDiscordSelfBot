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

#include "killcommand.hpp"

KillCommand::KillCommand(QDiscord& discord, QObject* parent)
: QObject(parent), Command(discord)
{

}

void KillCommand::dispatch(QDiscordMessage message, QStringList args)
{
	Q_UNUSED(args);
	//Just your average command to kill the application.
	_discord.rest()->deleteMessage(message);
	_discord.logout();
	connect(&_discord, &QDiscord::loggedOut, [=]() {
		qApp->exit(EXIT_SUCCESS);
	});
}

QString KillCommand::commandName()
{
	return "kill";
}

QString KillCommand::helpText()
{
	return "Kills the selfbot.";
}

QStringList KillCommand::argumentText()
{
	return QStringList();
}
