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

#ifndef URBANCOMMAND_HPP
#define URBANCOMMAND_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "command.hpp"

class UrbanCommand : public QObject, public Command
{
public:
	explicit UrbanCommand(QDiscord& discord, QObject* parent = 0);
	void dispatch(QDiscordMessage message, QStringList args);
	QString commandName();
	QString helpText();
	QStringList argumentText();
private:
	const QString _urbanEndpoint =
			"http://api.urbandictionary.com/v0/define?term=";
	QNetworkAccessManager _accessManager;
};

#endif // URBANCOMMAND_HPP
