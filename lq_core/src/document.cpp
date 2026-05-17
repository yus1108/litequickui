#include "lq_core/document.h"
#include "document.hpp"

#include "utf8.hpp"

litehtml::uint_ptr lq_document_container::create_font(const litehtml::font_description& descr, const litehtml::document* doc, litehtml::font_metrics* fm) noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.create_font != nullptr, "create_font callback must be provided.");

	lq_html_font_description descr_wrapper;
	descr_wrapper.family = lq_document_get_shared_str(_document, descr.family.c_str());
	descr_wrapper.size = descr.size;
	descr_wrapper.style = (lq_html_font_style)descr.style;
	descr_wrapper.weight = descr.weight;
	descr_wrapper.decoration_line = descr.decoration_line;
	descr_wrapper.decoration_thickness = descr.decoration_thickness.is_predefined() ? 
		lq_html_css_length_create_predef(descr.decoration_thickness.predef()) :
		lq_html_css_length_create_value(descr.decoration_thickness.val(), (lq_html_css_length_units)descr.decoration_thickness.units());
	descr_wrapper.decoration_style = (lq_html_text_decoration_style)descr.decoration_style;
	descr_wrapper.decoration_color = lq_color_create(descr.decoration_color.red, descr.decoration_color.green, descr.decoration_color.blue, descr.decoration_color.alpha);
	descr_wrapper.emphasis_style = lq_document_get_shared_str(_document, descr.emphasis_style.c_str());
	descr_wrapper.emphasis_color = lq_color_create(descr.emphasis_color.red, descr.emphasis_color.green, descr.emphasis_color.blue, descr.emphasis_color.alpha);
	descr_wrapper.emphasis_position = descr.emphasis_position;
	LQ_STATIC_ASSERT(sizeof(litehtml::font_description) == 136, FONT_DESCRIPTION_SIZE_CHANGED);

	return _callbacks.create_font(_document, &descr_wrapper, reinterpret_cast<lq_html_font_metrics_t*>(fm));
}

void lq_document_container::delete_font(litehtml::uint_ptr hFont) noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.delete_font != nullptr, "delete_font callback must be provided.");
	_callbacks.delete_font(_document, hFont);
}

const char* lq_document_container::get_default_font_name(void) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_default_font_name != nullptr, "get_default_font_name callback must be provided.");

	lq_utf8_str_t fontName = _callbacks.get_default_font_name(_document);
	LQ_DEBUG_ASSERT(fontName != nullptr, "get_default_font_name callback returned null.");
	return reinterpret_cast<const char*>(fontName->cstr);
}

litehtml::pixel_t lq_document_container::get_default_font_size(void) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_default_font_size != nullptr, "get_default_font_size callback must be provided.");
	return _callbacks.get_default_font_size(_document);
}

void lq_document_container::get_media_features(litehtml::media_features& media) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_media_features != nullptr, "Media features callback must be provided.");
	_callbacks.get_media_features(_document, reinterpret_cast<lq_html_media_features_t*>(&media));
	LQ_STATIC_ASSERT(sizeof(litehtml::media_features) == sizeof(lq_html_media_features_t), WRAPPER_STRUCT_SIZE_MISMATCH);
}

void lq_document_container::set_caption(const char* caption) noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.set_caption != nullptr, "set_caption callback must be provided.");
	_callbacks.set_caption(_document, lq_document_get_shared_str(_document, caption));
}

litehtml::pixel_t lq_document_container::text_width(const char* text, litehtml::uint_ptr hFont)
{
	LQ_DEBUG_ASSERT(_callbacks.calc_text_width != nullptr, "calc_text_width callback must be provided.");
	return _callbacks.calc_text_width(_document, lq_document_get_shared_str(_document, text), hFont);
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

lq_document_t lq_document_create(lq_utf8_str_t html_data, const lq_document_callbacks_t* callbacks, void* user_data)
{
	LQ_DEBUG_ASSERT(html_data != nullptr, "Input data must not be null.");

	lq_document_t doc = new struct lq_document();
	doc->container = new lq_document_container(doc, callbacks);
	doc->user_data = user_data;
	// Create document base from input HTML string encoded as UTF-8
	std::string htmlStr((const char*)html_data->cstr, html_data->size);
	doc->base = litehtml::document::createFromString(litehtml::estring(htmlStr, litehtml::encoding::utf_8, litehtml::confidence::certain), doc->container);

	return doc;
}

void lq_document_destroy(lq_document_t document)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");

	for (auto& pair : document->shared_strs)
	{
		lq_utf8_str_destroy(pair.second);
	}

	// Destroy document base before container, as document base may call back into container during destruction.
	lq_document_container* container = document->container;
	delete document;
	delete container;
}

void* lq_document_get_user_data(lq_document_t document)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");
	return document->user_data;
}

lq_utf8_str_t lq_document_get_shared_str(lq_document_t document, const lq_char_t* cstr)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");
	LQ_DEBUG_ASSERT(cstr != nullptr, "Input C-string must not be null.");

	auto pairIter = document->shared_strs.find(cstr);
	if (pairIter != document->shared_strs.end())
	{
		return pairIter->second;
	}

	lq_utf8_str_t utf8Str = lq_utf8_str_create(cstr);
	document->shared_strs.emplace(cstr, utf8Str);
	return utf8Str;
}
