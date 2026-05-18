#include "lq_core/document.h"
#include "document.hpp"

#include "utf8.hpp"

litehtml::uint_ptr lq_document_container::create_font(const litehtml::font_description& descr, const litehtml::document* doc, litehtml::font_metrics* fm) noexcept
{
	LQ_DEBUG_ASSERT(doc != nullptr, "Document pointer provided to create_font callback is null.");
	LQ_DEBUG_ASSERT(_callbacks.create_font != nullptr, "create_font callback must be provided.");

	lq_wrapper_font_description descr_wrapper;
	descr_wrapper.raw_utf8_family = descr.family.c_str();
	descr_wrapper.size = descr.size;
	descr_wrapper.style = (lq_wrapper_font_style)descr.style;
	descr_wrapper.weight = descr.weight;
	descr_wrapper.decoration_line = descr.decoration_line;
	descr_wrapper.decoration_thickness = descr.decoration_thickness.is_predefined() ? 
		lq_wrapper_css_length_create_predef(descr.decoration_thickness.predef()) :
		lq_wrapper_css_length_create_value(descr.decoration_thickness.val(), (lq_wrapper_css_length_units)descr.decoration_thickness.units());
	descr_wrapper.decoration_style = (lq_wrapper_text_decoration_style)descr.decoration_style;
	descr_wrapper.decoration_color = lq_color_create(descr.decoration_color.red, descr.decoration_color.green, descr.decoration_color.blue, descr.decoration_color.alpha);
	descr_wrapper.raw_utf8_emphasis_style = descr.emphasis_style.c_str();
	descr_wrapper.emphasis_color = lq_color_create(descr.emphasis_color.red, descr.emphasis_color.green, descr.emphasis_color.blue, descr.emphasis_color.alpha);
	descr_wrapper.emphasis_position = descr.emphasis_position;
	LQ_STATIC_ASSERT(sizeof(litehtml::font_description) == 136, FONT_DESCRIPTION_SIZE_CHANGED);

	return _callbacks.create_font(reinterpret_cast<lq_wrapper_font_metrics_t*>(fm), &descr_wrapper, reinterpret_cast<const lq_wrapper_document_t>(doc));
	LQ_STATIC_ASSERT(sizeof(litehtml::font_metrics) == sizeof(lq_wrapper_font_metrics_t), WRAPPER_STRUCT_SIZE_MISMATCH);
}

void lq_document_container::delete_font(litehtml::uint_ptr hFont) noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.delete_font != nullptr, "delete_font callback must be provided.");
	_callbacks.delete_font(hFont, lq_document_get_user_data(_document));
}

void lq_document_container::draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{
	LQ_DEBUG_ASSERT(text != nullptr, "Is this possible???");
	LQ_DEBUG_ASSERT(lq_inspect_utf8_cstr(nullptr, nullptr, text) == lq_true, "Is this possible???");
	LQ_DEBUG_ASSERT(_callbacks.draw_text != nullptr, "draw_text callback must be provided.");

	lq_color_t color_wrapper = lq_color_create(color.red, color.green, color.blue, color.alpha);

	_callbacks.draw_text(
		hdc,
		lq_cast_to_raw_utf8(text),
		hFont,
		&color_wrapper,
		reinterpret_cast<const lq_rect_t*>(&pos),
		lq_document_get_user_data(_document)
	);
}

const char* lq_document_container::get_default_font_name(void) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_default_font_name != nullptr, "get_default_font_name callback must be provided.");

	lq_utf8_str_t fontName = _callbacks.get_default_font_name(lq_document_get_user_data(_document));
	LQ_DEBUG_ASSERT(fontName != nullptr, "get_default_font_name callback returned null.");
	return lq_cast_to_cstr(fontName->cstr);
}

litehtml::pixel_t lq_document_container::get_default_font_size(void) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_default_font_size != nullptr, "get_default_font_size callback must be provided.");
	return _callbacks.get_default_font_size(lq_document_get_user_data(_document));
}

void lq_document_container::get_media_features(litehtml::media_features& media) const noexcept
{
	LQ_DEBUG_ASSERT(_callbacks.get_media_features != nullptr, "Media features callback must be provided.");
	_callbacks.get_media_features(reinterpret_cast<lq_wrapper_media_features_t*>(&media), lq_document_get_user_data(_document));
	LQ_STATIC_ASSERT(sizeof(litehtml::media_features) == sizeof(lq_wrapper_media_features_t), WRAPPER_STRUCT_SIZE_MISMATCH);
}

void lq_document_container::get_viewport(litehtml::position& viewport) const
{
	LQ_DEBUG_ASSERT(_callbacks.get_viewport != nullptr, "get_viewport callback must be provided.");
	_callbacks.get_viewport(reinterpret_cast<lq_rect_t*>(&viewport), lq_document_get_user_data(_document));
	LQ_STATIC_ASSERT(sizeof(litehtml::position) == sizeof(lq_rect_t), WRAPPER_STRUCT_SIZE_MISMATCH);
}

void lq_document_container::set_caption(const char* caption) noexcept
{
	LQ_DEBUG_ASSERT(caption != nullptr, "Is this possible???");
	LQ_DEBUG_ASSERT(lq_inspect_utf8_cstr(nullptr, nullptr, caption) == lq_true, "Is this possible???");
	LQ_DEBUG_ASSERT(_callbacks.set_caption != nullptr, "set_caption callback must be provided.");

	_callbacks.set_caption(lq_cast_to_raw_utf8(caption), lq_document_get_user_data(_document));
}

litehtml::pixel_t lq_document_container::text_width(const char* text, litehtml::uint_ptr hFont)
{
	LQ_DEBUG_ASSERT(text != nullptr, "Is this possible???");
	LQ_DEBUG_ASSERT(lq_inspect_utf8_cstr(nullptr, nullptr, text) == lq_true, "Is this possible???");
	LQ_DEBUG_ASSERT(_callbacks.calc_text_width != nullptr, "calc_text_width callback must be provided.");

	return _callbacks.calc_text_width(lq_cast_to_raw_utf8(text), hFont, lq_document_get_user_data(_document));
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

lq_document_t lq_document_create(const lq_utf8_str_t html_data, const lq_document_callbacks_t* callbacks, lq_uintptr_t user_data)
{
	LQ_DEBUG_ASSERT(html_data != nullptr, "Input data must not be null.");

	lq_document_t doc = new struct lq_document();
	doc->container = new lq_document_container(doc, callbacks);
	doc->user_data = user_data;
	// Create document base from input HTML string encoded as UTF-8
	std::string htmlStr(lq_cast_to_cstr(html_data->cstr), html_data->size);
	doc->base = litehtml::document::createFromString(litehtml::estring(htmlStr, litehtml::encoding::utf_8, litehtml::confidence::certain), doc->container);

	return doc;
}

void lq_document_destroy(lq_document_t document)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");

	// Destroy document base before container, as document base may call back into container during destruction.
	lq_document_container* container = document->container;
	delete document;
	delete container;
}

lq_uintptr_t lq_document_get_user_data(const lq_document_t document)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");
	return document->user_data;
}

lq_pixel_t lq_document_calc_layout(lq_document_t document, lq_pixel_t max_width, lq_wrapper_render_type render_type)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");
	return document->base->render(max_width, static_cast<litehtml::render_type>(render_type));
}

void lq_document_draw(lq_document_t document, lq_uintptr_t hdc, lq_pixel_t x, lq_pixel_t y, const lq_rect_t* clip)
{
	LQ_DEBUG_ASSERT(document != nullptr, "Input document must not be null.");
	document->base->draw(hdc, x, y, reinterpret_cast<const litehtml::position*>(clip));
	LQ_STATIC_ASSERT(sizeof(lq_rect_t) == sizeof(litehtml::position), WRAPPER_STRUCT_SIZE_MISMATCH);
}