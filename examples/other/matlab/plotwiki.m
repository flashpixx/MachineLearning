% ############################################################################
% # LGPL License                                                             #
% #                                                                          #
% # This file is part of the Machine Learning Framework.                     #
% # Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
% # This program is free software: you can redistribute it and/or modify     #
% # it under the terms of the GNU Lesser General Public License as           #
% # published by the Free Software Foundation, either version 3 of the       #
% # License, or (at your option) any later version.                          #
% #                                                                          #
% # This program is distributed in the hope that it will be useful,          #
% # but WITHOUT ANY WARRANTY; without even the implied warranty of           #
% # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
% # GNU Lesser General Public License for more details.                      #
% #                                                                          #
% # You should have received a copy of the GNU Lesser General Public License #
% # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
% ############################################################################


% plots a HDF file that is created by the "mds_wikipedia" example
% @param pcfile HDF file
% @param pnnodes number of nodes
function plotwiki( pcfile, pnnodes )
    lmarkersize=5;

    % get data (if only one file is used)
    if nargin < 2 || isempty(pnnodes)
        data  = hdf5read( pcfile, '/project');
        label = hdf5read( pcfile, '/label');
    else
        %if files of the cluster nodes are used
        loaddata  = cell(pnnodes,1);
        loadlabel = cell(pnnodes,1);
        for i=1:pnnodes
            loaddata{i}  = hdf5read( strcat('node', num2str(i-1), '_', pcfile), '/project');
            loadlabel{i} = hdf5read( strcat('node', num2str(i-1), '_', pcfile), '/label');
        end
        
        data  = cell2mat(loaddata);
        label = cell2mat(loadlabel);
    end
    
        
    if (size(data,2) ~= 2) && (size(data,2) ~= 3)
        error('plot only with 2D or 3D');
    end
    datatxt=cell(size(label,1), 1);
    for i=1:size(datatxt,1)
        datatxt{i} = char(label(i).data);
    end

    
    
    % create plot
    figure;
    grid on;
    hold on;
    set(datacursormode(gcf), 'DisplayStyle','datatip', 'SnapToDataVertex','off','Enable','on', 'UpdateFcn',{@showlabel,datatxt});
    
    
    if size(data,2) == 2
        plot(data(:,1), data(:,2), '.', 'MarkerSize', lmarkersize);
    elseif size(data,2) == 3
        plot3(data(:,1), data(:,2), data(:,3), '.', 'MarkerSize', lmarkersize);
    end
    set(gca,'fontsize',1);

    if size(data,2) == 3
        view([-45 45]);
    end
    


% label function for create "mouse over effect"
% @param varargin input data
% @return label text
function txt = showlabel(varargin)
    label = varargin{3};
    txt   = label{get(varargin{2}, 'DataIndex')};
