#pragma once
#ifndef TESTDLL_H
#define TESTDLL_H

#ifdef TESTDLL_EXPORTS
#define TESTDLL_API __declspec(dllexport)
#else
#define TESTDLL_API __declspec(dllimport)
#endif

#endif