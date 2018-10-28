package ro.ubb.pr102.web.converter;

import java.util.Collection;
import java.util.stream.Collectors;

public abstract class AbstractConverter<Model, Dto> implements Converter<Model, Dto> {
    public Collection<Dto> convertModelsToDtos(Collection<Model> models){
        return models.stream()
                .map(this::convertModelToDto)
                .collect(Collectors.toSet());
    }

    public Collection<Model> convertDtosToModels(Collection<Dto> dtos){
        return dtos.stream()
                .map(this::convertDtoToModel)
                .collect(Collectors.toSet());
    }
}