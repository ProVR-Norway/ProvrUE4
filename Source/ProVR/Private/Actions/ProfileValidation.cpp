// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital
#include "../../Public/Actions/ProfileValidation.h"
#include <regex>

bool UProfileValidation::IsEmailValid(const FString & email)
{
	// define a regular expression
	const std::regex pattern
	("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	// try to match the string with the regular expression
	return std::regex_match(TCHAR_TO_UTF8(*email), pattern);
}
FString UProfileValidation::PasswordStrength(const FString& password)

{
	std::string passwordString = std::string(TCHAR_TO_UTF8(*password));
	bool hasLower = false;
	bool hasUpper = false;
	bool hasDigit = false;
	bool hasSpecialChar = false;
	std::string normalChars = "abcdefghijklmnopqrstu" "vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";
	for (int32 i = 0; i < password.Len(); i++) {
		if (islower(password[i]))
			hasLower = true;

		if (isupper(password[i]))
			hasUpper = true;

		if (isdigit(password[i]))
			hasDigit = true;

		if (passwordString.find_first_not_of(normalChars) != std::string::npos)
			hasSpecialChar = true;
	}

	if (hasLower && hasUpper && hasDigit && hasSpecialChar && (password.Len() >= 8))
	{
		return "Strong";
	}
	else if (hasLower && hasUpper && hasDigit && (password.Len() >= 6))
	{
		return "Moderate";
	}
	else
		return "Weak";
}
