#include "lq_core.h"
#include "lq_core.hpp"

#include "lq_debug.hpp"

lq_utf8_string_t lq_utf8_string_create(lq_context_t context, const lq_byte_t* data)
{
	LQ_DEBUG_ASSERT(context != nullptr, "Input context must not be null.");
	LQ_DEBUG_ASSERT(data != nullptr, "Input data must not be null.");

	lq_uint32_t length, size;
	if (lq_utf8_string_inspect(&length, &size, data) == false) { return nullptr; }

	lq_utf8_string_t str = static_cast<lq_utf8_string_t>(malloc(sizeof(struct lq_utf8_string)));
	str->data = static_cast<lq_byte_t*>(malloc(size));
	str->length = length;
	str->size = size;
	str->context_index = static_cast<lq_uint32_t>(context->utf8_strings.size());
	context->utf8_strings.push_back(str);
	return str;
}

void lq_utf8_string_destroy(lq_context_t context, lq_utf8_string_t str)
{
	LQ_DEBUG_ASSERT(context != nullptr, "Input context must not be null.");
	LQ_DEBUG_ASSERT(str != nullptr, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->context_index < context->utf8_strings.size(), "String context index must be within bounds of context's utf8_strings vector.");
	LQ_DEBUG_ASSERT(str->data != nullptr, "String data must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");
	LQ_DEBUG_ASSERT(str->length > 0, "String length must be greater than 0.");

	lq_uint32_t index = str->context_index;
	free(str->data);
	free(str);


	if (str == context->utf8_strings.back())
	{
		context->utf8_strings.pop_back();
		return;
	}

	lq_utf8_string_t replacedStr = context->utf8_strings[index] = context->utf8_strings.back();
	replacedStr->context_index = index;
	context->utf8_strings.pop_back();
}

lq_uint32_t lq_utf8_string_length(lq_utf8_string_t str)
{
	LQ_DEBUG_ASSERT(str != nullptr, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->data != nullptr, "String data must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");
	LQ_DEBUG_ASSERT(str->length > 0, "String length must be greater than 0.");

	return str->length;
}

lq_uint32_t lq_utf8_string_size(lq_utf8_string_t str)
{
	LQ_DEBUG_ASSERT(str != nullptr, "Input string must not be null.");
	LQ_DEBUG_ASSERT(str->data != nullptr, "String data must not be null.");
	LQ_DEBUG_ASSERT(str->size > 0, "String size must be greater than 0.");
	LQ_DEBUG_ASSERT(str->length > 0, "String length must be greater than 0.");

	return str->size;
}

lq_context_t lq_context_create(void)
{
	return new struct lq_context();
}

void lq_context_destroy(lq_context_t context)
{
	LQ_DEBUG_ASSERT(context != nullptr, "Input context must not be null.");
	delete context;
}

litehtml::uint_ptr lq_document_container::create_font(const litehtml::font_description& descr, const litehtml::document* doc, litehtml::font_metrics* fm)
{
	LQ_DEBUG_ASSERT(false, "create_font is not implemented yet.");
	return 0;
}

void lq_document_container::delete_font(litehtml::uint_ptr hFont)
{
	LQ_DEBUG_ASSERT(false, "delete_font is not implemented yet.");
}

litehtml::pixel_t lq_document_container::get_default_font_size(void) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_default_font_size != nullptr, "get_default_font_size callback must be provided.");
	return _callbacks.get_default_font_size(_context, _document);
}

const char* lq_document_container::get_default_font_name(void) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_default_font_name != nullptr, "get_default_font_name callback must be provided.");

	lq_utf8_string_t fontName = _callbacks.get_default_font_name(_context, _document);
	LQ_DEBUG_ASSERT(fontName != nullptr, "get_default_font_name callback returned null.");
	return reinterpret_cast<const char*>(fontName->data);
}

void lq_document_container::get_media_features(litehtml::media_features& media) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_media_features != nullptr, "Media features callback must be provided.");
	_callbacks.get_media_features(_context, _document, reinterpret_cast<lq_media_features_t*>(&media));
	LQ_STATIC_ASSERT(sizeof(litehtml::media_features) == sizeof(lq_media_features_t), WRAPPER_STRUCT_SIZE_MISMATCH);
}

litehtml::pixel_t lq_document_container::text_width(const char* text, litehtml::uint_ptr hFont)
{
	LQ_DEBUG_ASSERT(false, "text_width is not implemented yet.");
	return 0.0f;
}

void lq_document_container::draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{
	LQ_DEBUG_ASSERT(false, "draw_text is not implemented yet.");
}

litehtml::pixel_t lq_document_container::pt_to_px(float pt) const
{
	LQ_DEBUG_ASSERT(false, "pt_to_px is not implemented yet.");
	return 0.0f;
}

void lq_document_container::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
	LQ_DEBUG_ASSERT(false, "draw_list_marker is not implemented yet.");
}

void lq_document_container::load_image(const char* src, const char* baseurl, bool redraw_on_ready)
{
	LQ_DEBUG_ASSERT(false, "load_image is not implemented yet.");
}

void lq_document_container::get_image_size(const char* src, const char* baseurl, litehtml::size& sz)
{
	LQ_DEBUG_ASSERT(false, "get_image_size is not implemented yet.");
}

void lq_document_container::draw_image(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const std::string& url, const std::string& base_url)
{
	LQ_DEBUG_ASSERT(false, "draw_image is not implemented yet.");
}

void lq_document_container::draw_solid_fill(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::web_color& color)
{
	LQ_DEBUG_ASSERT(false, "draw_solid_fill is not implemented yet.");
}

void lq_document_container::draw_linear_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::linear_gradient& gradient)
{
	LQ_DEBUG_ASSERT(false, "draw_linear_gradient is not implemented yet.");
}

void lq_document_container::draw_radial_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::radial_gradient& gradient)
{
	LQ_DEBUG_ASSERT(false, "draw_radial_gradient is not implemented yet.");
}

void lq_document_container::draw_conic_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::conic_gradient& gradient)
{
	LQ_DEBUG_ASSERT(false, "draw_conic_gradient is not implemented yet.");
}

void lq_document_container::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root)
{
	LQ_DEBUG_ASSERT(false, "draw_borders is not implemented yet.");
}

void lq_document_container::set_base_url(const char* base_url)
{
	LQ_DEBUG_ASSERT(false, "set_base_url is not implemented yet.");
}

void lq_document_container::set_caption(const char* caption)
{
	LQ_DEBUG_ASSERT(false, "set_caption is not implemented yet.");
}

void lq_document_container::link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el)
{
	LQ_DEBUG_ASSERT(false, "link is not implemented yet.");
}

void lq_document_container::on_anchor_click(const char* url, const litehtml::element::ptr& el)
{
	LQ_DEBUG_ASSERT(false, "on_anchor_click is not implemented yet.");
}

bool lq_document_container::on_element_click(const litehtml::element::ptr& el)
{
	LQ_DEBUG_ASSERT(false, "on_element_click is not implemented yet.");
	return false;
}

void lq_document_container::on_mouse_event(const litehtml::element::ptr& el, litehtml::mouse_event event)
{
	LQ_DEBUG_ASSERT(false, "on_mouse_event is not implemented yet.");
}

void lq_document_container::set_cursor(const char* cursor)
{
	LQ_DEBUG_ASSERT(false, "set_cursor is not implemented yet.");
}

void lq_document_container::transform_text(litehtml::string& text, litehtml::text_transform tt)
{
	LQ_DEBUG_ASSERT(false, "transform_text is not implemented yet.");
}

void lq_document_container::import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl)
{
	LQ_DEBUG_ASSERT(false, "import_css is not implemented yet.");
}

void lq_document_container::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius)
{
	LQ_DEBUG_ASSERT(false, "set_clip is not implemented yet.");
}

void lq_document_container::del_clip()
{
	LQ_DEBUG_ASSERT(false, "del_clip is not implemented yet.");
}

void lq_document_container::get_viewport(litehtml::position& viewport) const
{
	LQ_DEBUG_ASSERT(false, "get_viewport is not implemented yet.");
}

litehtml::element::ptr lq_document_container::create_element(const char* tag_name, const litehtml::string_map& attributes, const std::shared_ptr<litehtml::document>& doc)
{
	if (strcmp(tag_name, "script") == 0)
	{
		LQ_DEBUG_ASSERT(attributes.find("defer") != attributes.end(), "LiteHTML does not support synchronous <script> execution. Missing 'defer' attribute in <script> tag.");
		//_deferredScriptElements.emplace_back(std::make_shared<HTMLElementScript>(doc));
		//return _deferredScriptElements.back();
		LQ_DEBUG_ASSERT(false, "Need to implement <script> element with defer attribute.");
	}
	if (attributes.find("onclick") != attributes.end())
	{
		LQ_DEBUG_ASSERT(false, "Need to implement element.onclick");
	}
	return nullptr;
}

void lq_document_container::get_language(litehtml::string& language, litehtml::string& culture) const
{
	LQ_DEBUG_ASSERT(false, "get_language is not implemented yet.");
}

lq_document_t lq_document_create
(
	lq_context_t context, 
	lq_utf8_string_t html_data, 
	const lq_document_callbacks_t* callbacks, 
	void* user_data
)
{
	LQ_DEBUG_ASSERT(context != nullptr, "Input context must not be null.");
	LQ_DEBUG_ASSERT(html_data != nullptr, "Input data must not be null.");
	LQ_DEBUG_ASSERT(context->utf8_strings.size() > html_data->context_index, "String context index must be within bounds of context's utf8_strings vector.");
	LQ_DEBUG_ASSERT(context->utf8_strings[html_data->context_index] == html_data, "Input string must be the same as the one stored in context.");

	lq_document_t doc = static_cast<lq_document_t>(malloc(sizeof(struct lq_document)));
	doc->container = new lq_document_container(context, doc, callbacks);
	doc->context_index = static_cast<lq_uint32_t>(context->documents.size());
	doc->user_data = user_data;
	// Create document base from input HTML string encoded as UTF-8
	std::string htmlStr((const char*)html_data->data, html_data->size);
	doc->base = litehtml::document::createFromString(litehtml::estring(htmlStr, litehtml::encoding::utf_8, litehtml::confidence::certain), doc->container);

	context->documents.emplace_back(doc);
	return doc;
}

void lq_document_destroy(lq_context_t context, lq_document_t document)
{
	LQ_DEBUG_ASSERT(context != nullptr, "Input context must not be null.");
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");

	lq_uint32_t index = document->context_index;
	delete document->container;
	free(document);

	if (document == context->documents.back())
	{
		context->documents.pop_back();
		return;
	}

	lq_document_t replacedDoc = context->documents[index] = context->documents.back();
	replacedDoc->context_index = index;
	context->documents.pop_back();
}

lq_bool_t lq_utf8_string_inspect(lq_uint32_t* out_opt_length, lq_uint32_t* out_opt_size, const lq_byte_t* data)
{
	if (data == nullptr) return false;

	lq_uint32_t length = 0;
	const lq_byte_t* curr = data;

	while (*curr != 0)
	{
		lq_uint32_t expectedBytes = 0;
		lq_byte_t firstByte = *curr;

		if ((firstByte & 0x80) == 0x00) { expectedBytes = 1; }
		else if ((firstByte & 0xE0) == 0xC0) { expectedBytes = 2; }
		else if ((firstByte & 0xF0) == 0xE0) { expectedBytes = 3; }
		else if ((firstByte & 0xF8) == 0xF0) { expectedBytes = 4; }
		else { return false; }

		lq_uint32_t codePoint = 0;
		switch (expectedBytes)
		{
		case 4:
			if (curr[1] == 0 || curr[2] == 0 || curr[3] == 0) { return false; }
			if ((curr[3] & 0xC0) != 0x80) { return false; }
			if ((curr[2] & 0xC0) != 0x80) { return false; }
			if ((curr[1] & 0xC0) != 0x80) { return false; }
			codePoint = ((curr[0] & 0x07) << 18) | ((curr[1] & 0x3F) << 12) | ((curr[2] & 0x3F) << 6) | (curr[3] & 0x3F);
			// Check Range & Overlong
			if (codePoint < 0x10000 || codePoint > 0x10FFFF) { return false; }
			break;
		case 3:
			if (curr[1] == 0 || curr[2] == 0) { return false; }
			if ((curr[2] & 0xC0) != 0x80) { return false; }
			if ((curr[1] & 0xC0) != 0x80) { return false; }
			codePoint = ((curr[0] & 0x0F) << 12) | ((curr[1] & 0x3F) << 6) | (curr[2] & 0x3F);
			// Check Overlong & Surrogate Pair Range
			if (codePoint < 0x0800 || (codePoint >= 0xD800 && codePoint <= 0xDFFF)) { return false; }
			break;
		case 2:
			if (curr[1] == 0) { return false; }
			if ((curr[1] & 0xC0) != 0x80) { return false; }
			codePoint = ((curr[0] & 0x1F) << 6) | (curr[1] & 0x3F);
			if (codePoint < 0x0080) { return false; } // Check Overlong
			break;
		case 1:
			break;
		default:
			return false;
		}

		curr += expectedBytes;
		length++;
	}

	if (out_opt_length != nullptr) { *out_opt_length = length; }
	if (out_opt_size != nullptr) { *out_opt_size = static_cast<lq_uint32_t>(curr - data) + 1; }

	return true;
}

void* lq_document_get_user_data(lq_document_t document)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");
	return document->user_data;
}
