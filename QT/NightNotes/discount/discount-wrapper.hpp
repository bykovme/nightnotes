/* discount-wrapper.hpp
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


#ifndef DISCOUNT_WRAPPER_HPP_
#define DISCOUNT_WRAPPER_HPP_

#include <map>
#include <string>

// Discount flags are preprocessor macro defined in mkdio.h which is not
// included in this header to avoid namespace "pollution". The following enum
// is mapped by the wrapper to the actual discount flags.
//
// See discount documentation for details about flags
enum class MkdFlags {
	NOLINKS,
	NOIMAGE,
	NOPANTS,
	NOHTML,
	STRICT,
	TAGTEXT,
	NO_EXT,
	CDATA,
	NOSUPERSCRIPT,
	NORELAXED,
	NOTABLES,
	NOSTRIKETHROUGH,
	TOC,
	v1_COMPAT,
	AUTOLINK,
	SAFELINK,
	NOHEADER,
	TABSTOP,
	NODIVQUOTE,
	NOALPHALIST,
	NODLIST,
	EXTRA_FOOTNOTE,
	NOSTYLE,
	EMBED
};

class Markdown
{
public:
	// Construct and set a markdown string
	explicit Markdown(const std::string& markdown = "") noexcept;
	// Get CSS style if any (discount mkd_css() function)
	const std::string& css() const noexcept;
	// Get HTML document (discount mkd_document() function)
	const std::string& html() const noexcept;
	// Get the markdown string
	const std::string& markdown() const noexcept;
	// Get table of content if any (discount mkd_toc() function)
	const std::string& toc() const noexcept;
	// Pandoc-style document headers (discount mkd_doc_title(),
	// mkd_doc_author(), and mkd_doc_date() functions)
	const std::string& author() const noexcept;
	const std::string& date() const noexcept;
	const std::string& title() const noexcept;
	void setMarkdown(const std::string& markdown) noexcept;
	// Compile Markdown string set with setMarkdown() or with constructor
	void compile();
	// Check if flag 'flag' is  set
	bool has(MkdFlags flag) const noexcept;
	// Clear all flags, ie. reset to 0
	void clearMkdFlags() noexcept;
	// Enable one or more flags at a time, by example:
	// setMkdFlags(MkdFlags::TOC, MkdFlags::NOHEADER, MkdFlags::NOHTML)
	template<typename ... Types>
	unsigned long int setMkdFlags(MkdFlags flag, Types ... more_flags) noexcept
	{
		// GO GO recursion!
		return mkd_flags_ = mkd_flags_map_.at(flag)
			| setMkdFlags(more_flags...);
	}
	unsigned long int setMkdFlags(MkdFlags flag) noexcept
	{
		return mkd_flags_ = mkd_flags_map_.at(flag);
	}

private:
	std::string css_;
	std::string html_;
	std::string markdown_;
	std::string toc_;
	std::string author_;
	std::string date_;
	std::string title_;
	unsigned long int mkd_flags_;

	static const std::string error_msg_mkd_string;
	static const std::map<MkdFlags, const unsigned long int> mkd_flags_map_;
};

#endif // DISCOUNT_WRAPPER_HPP_

