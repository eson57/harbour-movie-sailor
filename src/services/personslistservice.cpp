#include "personslistservice.h"

PersonsListService::PersonsListService(Api &api,
                                       FileCache &cache,
                                       Settings &settings,
                                       SearchPeopleListModel *anyRoleList,
                                       SearchPeopleListModel *castRoleList,
                                       SearchPeopleListModel *crewRoleList,
                                       QObject *parent) :
    QObject(parent),
    api(api),
    cache(cache),
    settings(settings),
    key("search", "persons", "1"),
    form(new SearchPeopleForm(this)),
    searchPersonListModel(new SearchPersonListModel(this)),
    anyRoleList(anyRoleList),
    castRoleList(castRoleList),
    crewRoleList(crewRoleList),
    initialized(false)
{
    connect(&api, &Api::searchPersonsDone, this, &PersonsListService::apiRequestDone);
}

void PersonsListService::search()
{
    QString newKey = form->getQuery() + "|" + (form->getWithAdult() ? "true" : "false") + "|" + settings.getLanguage() + "|" + QString::number(form->getPage());
    qDebug() << "initialize search person list with" << newKey;

    if (key.key != newKey) {
        key.key = newKey;
        searchPersonListModel->clear();
        setInitialized(false);
    }

    if (form->getQuery().isEmpty() || initialized) {
        return;
    }

    if (cache.exists(key)) {
        searchPersonListModel->fillFromCache(cache.load(key));
        setInitialized(true);
        return;
    }

    api.loadSearchPersons(*form);
}

SearchPersonListModel *PersonsListService::getSearchPersonListModel()
{
    return searchPersonListModel;
}

void PersonsListService::setInitialized(bool newInitialized)
{
    if (initialized == newInitialized)
        return;
    initialized = newInitialized;
    emit initializedChanged();
}

bool PersonsListService::isInitialized()
{
    return initialized;
}

SearchPeopleForm *PersonsListService::getForm() const
{
    return form;
}

void PersonsListService::select(int id)
{
    qDebug() << "select the person" << id;
    const QList<SearchPersonListItem> &persons = searchPersonListModel->getItems();
    for (QList<SearchPersonListItem>::const_iterator it = persons.constBegin(); it != persons.constEnd(); it++) {
        if (id == it->getId()) {
            qDebug() << "found a person to select in search list";
            qDebug() << "role" << it->getRole();
            if (it->getRole() == SearchPersonListItem::AnyRole) {
                anyRoleList->add(*it);
                castRoleList->remove(id);
                crewRoleList->remove(id);
                return;
            }
            if (it->getRole() == SearchPersonListItem::CastRole) {
                anyRoleList->remove(id);
                castRoleList->add(*it);
                crewRoleList->remove(id);
                return;
            }
            if (it->getRole() == SearchPersonListItem::CrewRole) {
                anyRoleList->remove(id);
                castRoleList->remove(id);
                crewRoleList->add(*it);
                return;
            }
            return;
        }
    }
}

void PersonsListService::remove(int id)
{
    qDebug() << "remove the person" << id;
    if (anyRoleList->getIds().contains(id))
        anyRoleList->remove(id);
    else if (castRoleList->getIds().contains(id))
        castRoleList->remove(id);
    else if (crewRoleList->getIds().contains(id))
        crewRoleList->remove(id);
    searchPersonListModel->resetRole(id);
}

void PersonsListService::apiRequestDone(const QByteArray &data)
{
    QJsonDocument apiJson = QJsonDocument::fromJson(data);
    QMap<SearchPersonListItem::PersonRole, QList<int>> selectedPeoplePerRole;
    selectedPeoplePerRole.insert(SearchPersonListItem::AnyRole, anyRoleList->getIds());
    selectedPeoplePerRole.insert(SearchPersonListItem::CastRole, castRoleList->getIds());
    selectedPeoplePerRole.insert(SearchPersonListItem::CrewRole, crewRoleList->getIds());
    QJsonDocument newJson = searchPersonListModel->fillFromAPI(apiJson, selectedPeoplePerRole);
    cache.save(key, newJson);
    setInitialized(true);
}
