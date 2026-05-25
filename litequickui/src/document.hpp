#pragma once
#include "litequickui/document.h"

typedef struct lq_document
{
	lq_core_document_t core;
	lq_core_document_callbacks_t callbacks;
}* lq_document_t;