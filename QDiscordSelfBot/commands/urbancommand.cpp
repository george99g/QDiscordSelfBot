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

#include "urbancommand.hpp"

UrbanCommand::UrbanCommand(QDiscord& discord, QObject* parent)
:QObject(parent), Command(discord)
{

}

void UrbanCommand::dispatch(QDiscordMessage message, QStringList args)
{
	if(args.length() != 1)
	{
		_discord.rest()->editMessage("One argument is required.", message);
		return;
	}
	//Construct the URL
	QUrl url(_urbanEndpoint + args[0]);
	//Construct our request
	QNetworkRequest request(url);
	//Tell the access manager to GET our request.
	QNetworkReply* reply = _accessManager.get(request);
	//Connect the reply with a lambda so we can process it once it's done.
	//Notice that our lambda captures by copy instead of by reference.
	//This method is going to end and everything in scope will be destroyed,
	//after which the lambda will be called once the request is done.
	//If the lambda captured by reference, those references wouldn't really be
	//pointing to valid objects.
	connect(reply, &QNetworkReply::finished,
	[=]() {
		if(reply->error() != QNetworkReply::NoError)
		{
			_discord.rest()->editMessage("Error: " + reply->errorString(), message);
			reply->deleteLater();
			return;
		}
		QJsonObject object = QJsonDocument::fromJson(reply->readAll()).object();
		if(
				!object.contains("result_type") ||
				object["result_type"].toString() != "exact" ||
				!object.contains("list")
		  )
		{
			_discord.rest()->editMessage("Could not find a definition.", message);
			reply->deleteLater();
			return;
		}
		object = object["list"].toArray().first().toObject();
		QString definition = "**" + args[0] + ":**\n";
		definition += "**Definition:**\n" + object["definition"].toString() + "\n";
		definition += "**Example:**\n" + object["example"].toString() + "\n";
		definition += "**Link:** <" + object["permalink"].toString() + ">";
		if(definition.length() > 2000)
			definition = "Definition too long. Visit " + object["permalink"].toString();
		_discord.rest()->editMessage(definition, message);
		reply->deleteLater();
	});
}

QString UrbanCommand::commandName()
{
	return "urban";
}

QString UrbanCommand::helpText()
{
	return "Searches <http://urbandictionary.com> for the specified text.";
}

QStringList UrbanCommand::argumentText()
{
	return {"(TEXT)", "TEXT is the query."};
}
