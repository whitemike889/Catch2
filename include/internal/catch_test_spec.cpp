/*
 *  Created by Martin on 19/07/2017.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "catch_test_spec.h"
#include "catch_string_manip.h"

#include <algorithm>
#include <string>
#include <vector>
#include <memory>

namespace Catch {

    TestSpec::Pattern::~Pattern() = default;
    TestSpec::NamePattern::~NamePattern() = default;
    TestSpec::TagPattern::~TagPattern() = default;

    TestSpec::NamePattern::NamePattern( std::string const& name )
    : m_wildcardPattern( toLower( name ), CaseSensitive::No )
    {}
    bool TestSpec::NamePattern::matches( TestCaseInfo const& testCase ) const {
        return m_wildcardPattern.matches( toLower( testCase.name ) );
    }

    TestSpec::TagPattern::TagPattern( std::string const& tag ) : m_tag( toLower( tag ) ) {}
    bool TestSpec::TagPattern::matches( TestCaseInfo const& testCase ) const {
        return std::find(begin(testCase.lcaseTags),
                         end(testCase.lcaseTags),
                         m_tag) != end(testCase.lcaseTags);
    }

    bool TestSpec::Filter::matches( TestCaseInfo const& testCase ) const {
        bool should_use = !testCase.isHidden();
        for (auto const& pattern : m_required) {
            should_use = true;
            if (!pattern->matches(testCase)) {
                return false;
            }
        }
        for (auto const& pattern : m_forbidden) {
            if (pattern->matches(testCase)) {
                return false;
            }
        }
        return should_use;
    }

    bool TestSpec::hasFilters() const {
        return !m_filters.empty();
    }
    bool TestSpec::matches( TestCaseInfo const& testCase ) const {
        // A TestSpec matches if any filter matches
        for( auto const& filter : m_filters )
            if( filter.matches( testCase ) )
                return true;
        return false;
    }
}
