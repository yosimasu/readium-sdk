//
//  iri.h
//  ePub3
//
//  Created by Jim Dovey on 2013-01-15.
//  Copyright (c) 2012-2013 The Readium Foundation and contributors.
//  
//  The Readium SDK is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef __ePub3__iri__
#define __ePub3__iri__

#include "utfstring.h"
#include "gurl.h"
#include <vector>

EPUB3_BEGIN_NAMESPACE

class CFI;

class IRI
{
    // would like this to contain const strings, but that proves awkward for now
    typedef std::vector<string>         ComponentList;
    typedef ComponentList::size_type    size_type;
    
    static string gPathSeparator;
    static string gURNScheme;
    static string gReservedCharacters;
    
public:
    typedef const std::pair<string, string> IRICredentials;
    
    static string gEPUBScheme;
    
public:
    IRI() : _urnComponents(), _url(nullptr), _pureIRI() {}
    
    // create from an IRI or URI string of any (valid) kind
    IRI(const string& iriStr);
    // create a URN
    IRI(const string& nameID, const string& namespacedString);
    // create a simple URL
    IRI(const string& scheme, const string& host, const string& path, const string& query="", const string& fragment="");
    
    IRI(const IRI& o) : _urnComponents(o._urnComponents), _url(new GURL(*o._url)), _pureIRI(o._pureIRI) {}
    IRI(IRI&& o) : _urnComponents(std::move(o._urnComponents)), _url(o._url), _pureIRI(std::move(o._pureIRI)) { o._url = nullptr; }
    
    virtual ~IRI();
    
    IRI&            operator=(const IRI& o);
    IRI&            operator=(IRI&& o);
    
    bool            operator==(const IRI& o)                const;
    bool            operator!=(const IRI& o)                const;
    
    bool            operator<(const IRI& o)                 const;
    
    bool            IsURN() const { return _urnComponents.size() > 1; }
    bool            IsRelative() const { return !_url->has_host(); }
    const string    Scheme() const { return (IsURN() ? _urnComponents[0] : _url->scheme()); }    // simple, because it must ALWAYS be present (even if empty, as for pure fragment IRIs)
    const string&   NameID() const { if (!IsURN()) { throw std::invalid_argument("Not a URN"); } return _urnComponents[1]; }
    const string    Host() const { return _url->host(); }
    IRICredentials  Credentials() const;
    const string    NamespacedString() const { if (!IsURN()) { throw std::invalid_argument("Not a URN"); } return _urnComponents[2]; }
    int             Port() const { return _url->EffectiveIntPort(); }
    const string    Path(bool URLEncoded=true) const;
    const string    Query() const { return _url->query(); }
    const string    Fragment() const { return _url->ref(); }
    const string    LastPathComponent() const { return _url->ExtractFileName(); }
    const CFI       ContentFragmentIdentifier() const;
    
    void            SetScheme(const string& scheme);
    void            SetHost(const string& host);
    void            SetCredentials(const string& user, const string& pass);
    void            AddPathComponent(const string& component);
    void            SetQuery(const string& query);
    void            SetFragment(const string& query);
    void            SetContentFragmentIdentifier(const CFI& cfi);
    
    static string   URLEncodeComponent(const string& str);
    static string   PercentEncodeUCS(const string& str);
    static string   IDNEncodeHostname(const string& host);
    
    // percent-encodes characters only where required to keep the URL correct, IDN-encodes hostname
    string          IRIString() const;
    // percent-encodes all URL characters and invalid characters as UTF-8, IDN-encodes hostname
    string          URIString() const;
    
protected:
    ComponentList   _urnComponents;
    GURL*           _url;
    string          _pureIRI;       // may be empty
    
};

template <class _CharT, class _Traits>
inline std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& __os, const IRI& iri) {
    return __os << iri.URIString();
}

EPUB3_END_NAMESPACE

#endif /* defined(__ePub3__iri__) */
