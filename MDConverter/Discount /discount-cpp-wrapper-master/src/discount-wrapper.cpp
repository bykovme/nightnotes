/* discount-wrapper.cpp
 *
 * Copyright (C) 2013 Andrea Blankenstijn <a.b@kaze.ch>
 *
 * This file is part of discount-cpp-wrapper.
 *
 * discount-cpp-wrapper is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with discount-cpp-wrapper. If not, see
 * <http://www.gnu.org/licenses/>.
 */

extern "C" {
#include <mkdio.h>
}
#include <stdexcept>

#include "discount-wrapper.hpp"

const std::string Markdown::error_msg_mkd_string = "libmarkdown failed to "
	"parse the following Markdown:\n";

const std::map<MkdFlags, const unsigned long int> Markdown::mkd_flags_map_ {
	{MkdFlags::NOLINKS, MKD_NOLINKS},
	{MkdFlags::NOIMAGE, MKD_NOIMAGE},
	{MkdFlags::NOPANTS, MKD_NOPANTS},
	{MkdFlags::NOHTML, MKD_NOHTML},
	{MkdFlags::STRICT, MKD_STRICT},
	{MkdFlags::TAGTEXT, MKD_TAGTEXT},
	{MkdFlags::NO_EXT, MKD_NO_EXT},
	{MkdFlags::CDATA, MKD_CDATA},
	{MkdFlags::NOSUPERSCRIPT, MKD_NOSUPERSCRIPT},
	{MkdFlags::NORELAXED, MKD_NORELAXED},
	{MkdFlags::NOTABLES, MKD_NOTABLES},
	{MkdFlags::NOSTRIKETHROUGH, MKD_NOSTRIKETHROUGH},
	{MkdFlags::TOC, MKD_TOC},
	{MkdFlags::v1_COMPAT, MKD_1_COMPAT},
	{MkdFlags::AUTOLINK, MKD_AUTOLINK},
	{MkdFlags::SAFELINK, MKD_SAFELINK},
	{MkdFlags::NOHEADER, MKD_NOHEADER},
	{MkdFlags::TABSTOP, MKD_TABSTOP},
	{MkdFlags::NODIVQUOTE, MKD_NODIVQUOTE},
	{MkdFlags::NOALPHALIST, MKD_NOALPHALIST},
	{MkdFlags::NODLIST, MKD_NODLIST},
	{MkdFlags::EXTRA_FOOTNOTE, MKD_EXTRA_FOOTNOTE},
	{MkdFlags::NOSTYLE, MKD_NOSTYLE},
	{MkdFlags::EMBED, MKD_EMBED}
};

Markdown::Markdown(const std::string& markdown) noexcept
	: markdown_(markdown), mkd_flags_(0)
{
}

const std::string& Markdown::css() const noexcept
{
	return css_;
}

const std::string& Markdown::html() const noexcept
{
	return html_;
}

const std::string& Markdown::markdown() const noexcept
{
	return markdown_;
}

const std::string& Markdown::toc() const noexcept
{
	return toc_;
}

const std::string& Markdown::author() const noexcept
{
	return author_;
}

const std::string& Markdown::date() const noexcept
{
	return date_;
}

const std::string& Markdown::title() const noexcept
{
	return title_;
}

bool Markdown::has(MkdFlags flag) const noexcept
{
	return mkd_flags_ & mkd_flags_map_.at(flag);
}

void Markdown::clearMkdFlags() noexcept
{
	mkd_flags_ = 0;
}

void Markdown::setMarkdown(const std::string& markdown) noexcept
{
	markdown_.assign(markdown);
}

void Markdown::compile()
{
	MMIOT* doc = mkd_string(markdown_.c_str(), markdown_.size(), mkd_flags_);
	if(doc == nullptr)
		throw std::runtime_error(
				std::string(error_msg_mkd_string)
					.append(markdown_)
				);
	mkd_compile(doc, mkd_flags_);

	char *cstr_buff;
	int size = mkd_document(doc, &cstr_buff);
	html_.assign(cstr_buff, size);

	if(!has(MkdFlags::NOSTYLE))
	{
		size = mkd_css(doc, &cstr_buff);
		css_.assign(cstr_buff, size);
	}

	if(!(has(MkdFlags::NOHEADER) || has(MkdFlags::STRICT)))
	{
		author_ = mkd_doc_author(doc) == nullptr ? "" : mkd_doc_author(doc);
		date_ = mkd_doc_date(doc) == nullptr ? "" : mkd_doc_date(doc);
		title_ = mkd_doc_title(doc) == nullptr ? "" : mkd_doc_title(doc);
	}

	if(has(MkdFlags::TOC))
	{
		size = mkd_toc(doc, &cstr_buff);
		toc_.assign(cstr_buff, size);
	}

	mkd_cleanup(doc);
}

